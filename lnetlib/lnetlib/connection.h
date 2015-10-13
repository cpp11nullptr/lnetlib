/*

The MIT License (MIT)

Copyright (c) 2015 Ievgen Polyvanyi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef LNETLIB_CONNECTION_H
#define LNETLIB_CONNECTION_H

#include "config.h"

#include <mutex>
#include <memory>
#include <thread>
#include <map>
#include <queue>
#include <functional>

#include "asio/error.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/buffer.hpp"
#include "asio/read.hpp"
#include "asio/write.hpp"
#include "asio/streambuf.hpp"

#include "lsignal/lsignal.h"

#include "istream.h"
#include "ostream.h"
#include "socket.h"

namespace lnetlib
{
	class connection
		: public std::enable_shared_from_this<connection>
	{
	public:
		enum custom_error
		{
			no_such_file_or_directory = 2,	// ENOENT
			ssl_short_read = 335544539		// SSL_R_SHORT_READ
		};

		using error_code = asio::error_code;
		using tcp = asio::ip::tcp;
		using data_buffer = asio::const_buffers_1;
		using stream_buffer = asio::streambuf;

		using package_buffer = std::vector<stream_buffer::const_buffers_type>;
		using callback = std::function<void(std::unique_ptr<istream>)>;
		using dispatch_slot = std::function<void(std::unique_ptr<stream_buffer> buffer)>;

		lsignal::signal<void(std::shared_ptr<connection>)> closed;
		lsignal::signal<void(std::shared_ptr<connection>, int, const std::string&)> error;
		lsignal::signal<void(std::shared_ptr<connection>, std::unique_ptr<istream>)> received;

		connection(std::shared_ptr<socket> sckt);
		~connection();

		bool is_open() const;
		void close();

		template<typename T>
		std::unique_ptr<ostream> create_stream(T command);

		template<typename T>
		std::unique_ptr<ostream> create_stream(T command, callback cb);

	private:
		struct package
		{
			std::unique_ptr<package_buffer> buffer;
			std::unique_ptr<stream_buffer> size_buffer;
			std::unique_ptr<stream_buffer> data_buffer;
		};

		std::mutex _mutex;
		std::shared_ptr<socket> _socket;
		bool _socket_locked;
		std::queue<std::shared_ptr<package>> _packages;
		uint64_t _uid_counter;
		std::map<uint64_t, callback> _callbacks;

		bool check_error(const error_code& err);

		void wait_package();
		void wait_package_body(uint64_t size);

		void read_package_size_handler(std::shared_ptr<stream_buffer> buffer, const error_code& err, std::size_t bytes);
		void read_package_body_handler(std::shared_ptr<stream_buffer> buffer, const error_code& err, std::size_t bytes);

		std::shared_ptr<package> create_package(std::unique_ptr<stream_buffer> buffer) const;
		void send_package(std::shared_ptr<package> pkg);

		dispatch_slot create_dispatch_slot();

	};

	template<typename T>
	std::unique_ptr<ostream> connection::create_stream(T command)
	{
		std::unique_ptr<ostream> stream { new ostream(command, _uid_counter++) };

		stream->dispatch.connect(create_dispatch_slot());

		return stream;
	}

	template<typename T>
	std::unique_ptr<ostream> connection::create_stream(T command, callback cb)
	{
		std::unique_ptr<ostream> stream = create_stream(command);

		_callbacks[stream->uid()] = cb;

		return stream;
	}
}

#endif // LNETLIB_CONNECTION_H

