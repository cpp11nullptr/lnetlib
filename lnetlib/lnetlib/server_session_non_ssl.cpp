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

#include "server_session_non_ssl.h"

namespace lnetlib
{
	server_session_non_ssl::server_session_non_ssl(service& srv, std::shared_ptr<tcp::acceptor> acceptor)
		: _socket(new tcp::socket(srv)), _acceptor(acceptor)
	{
	}

	server_session_non_ssl::~server_session_non_ssl()
	{
	}

	void server_session_non_ssl::start()
	{
		auto handler = std::bind(&server_session_non_ssl::accept_handler, this, std::placeholders::_1);

		_acceptor->async_accept(*_socket.get(), handler);
	}

	void server_session_non_ssl::accept_handler(const error_code& err)
	{
		std::shared_ptr<connection> conn;

		if (!err)
		{
			std::shared_ptr<socket> sckt = std::make_shared<socket_non_ssl>(_socket);

			conn = std::make_shared<connection>(sckt);
		}

		connected(conn, err);
	}
}
