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

#ifndef LNETLIB_SOCKET_SSL_H
#define LNETLIB_SOCKET_SSL_H

#include "config.h"

#include "asio/ip/tcp.hpp"
#include "asio/ssl/stream.hpp"

#include "socket.h"

namespace lnetlib
{
	class socket_ssl
		: public socket
	{
	public:
		using tcp = asio::ip::tcp;
		using socket_base = asio::socket_base;
		using ssl_socket = asio::ssl::stream<tcp::socket>;

		socket_ssl(std::shared_ptr<ssl_socket> socket);
		~socket_ssl() override;

		bool is_open() const override;
		void close() override;

		void async_read(stream_buffer& buffer, completion_cond cond, async_read_handler handler) override;
		void async_write(const package_buffer& buffer, async_write_handler handler) override;

	private:
		std::shared_ptr<ssl_socket> _socket;

	};
}

#endif // LNETLIB_SOCKET_SSL_H

