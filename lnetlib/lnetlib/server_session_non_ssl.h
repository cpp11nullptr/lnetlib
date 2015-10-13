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

#ifndef LNETLIB_SERVER_SESSION_NON_SSL_H
#define LNETLIB_SERVER_SESSION_NON_SSL_H

#include "config.h"

#include <functional>

#include "server_session.h"
#include "socket_non_ssl.h"

namespace lnetlib
{
	class server_session_non_ssl
		: public server_session
	{
	public:
		using error_code = asio::error_code;
		using service = asio::io_service;
		using tcp = asio::ip::tcp;

		server_session_non_ssl(service& srv, std::shared_ptr<tcp::acceptor> acceptor);
		~server_session_non_ssl() override;

		void start() override;

	private:
		std::shared_ptr<tcp::socket> _socket;
		std::shared_ptr<tcp::acceptor> _acceptor;

		void accept_handler(const error_code& err);

	};
}

#endif // LNETLIB_SERVER_SESSION_NON_SSL_H

