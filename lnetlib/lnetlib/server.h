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

#ifndef LNETLIB_SERVER_H
#define LNETLIB_SERVER_H

#include "config.h"

#include <list>
#include <memory>
#include <mutex>
#include <thread>

#include "asio/error.hpp"
#include "asio/io_service.hpp"
#include "asio/ip/tcp.hpp"

#include "lsignal/lsignal.h"

#include "helper.h"
#include "istream.h"
#include "ostream.h"

#include "encryption.h"
#include "connection.h"

#include "server_session.h"
#include "server_session_ssl.h"
#include "server_session_non_ssl.h"

namespace lnetlib
{
	enum class server_thread_policy
	{
		one_per_connection,
		fixed_count
	};

	class server
	{
	public:
		using system_error = asio::system_error;
		using error_code = asio::error_code;
		using service = asio::io_service;
		using tcp = asio::ip::tcp;
		using address = asio::ip::address;

		lsignal::signal<void(std::shared_ptr<connection>)> connected;
		lsignal::signal<void(std::shared_ptr<connection>)> disconnected;
		lsignal::signal<void(std::shared_ptr<connection>, int, const std::string&)> error;
		lsignal::signal<void(std::shared_ptr<connection>, std::unique_ptr<istream>)> received;

		server();
		~server();

		server_thread_policy thread_policy() const;
		void set_thread_policy(server_thread_policy thread_policy, int32_t thread_count = -1);

		std::shared_ptr<encryption> encryption_info() const;

		void start(const std::string& addr, int port);
		void stop();

		std::list<std::shared_ptr<connection>> connections();

	private:
		bool _running;
		std::mutex _mutex;
		server_thread_policy _thread_policy;
		int32_t _thread_count;

		service _service;
		tcp::endpoint _endpoint;
		std::shared_ptr<tcp::acceptor> _acceptor;

		std::shared_ptr<encryption> _encryption;
		std::list<std::shared_ptr<connection>> _connections;
		std::map<std::shared_ptr<connection>, std::shared_ptr<server_session>> _sessions;

		void wait_first_connection();
		void wait_next_connection();

		void wait_connection(bool first = false);

		void create_session();
		void connected_handler(server_session *session, std::shared_ptr<connection> conn, const error_code& err);

		void add_connection(std::shared_ptr<connection> conn, std::shared_ptr<server_session> session);
		void remove_connection(std::shared_ptr<connection> conn);

		void init_service_threads();
		void create_service_thread();

	};
}

#endif // LNETLIB_SERVER_H
