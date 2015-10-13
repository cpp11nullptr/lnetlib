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

#include "server_session_ssl.h"

namespace lnetlib
{
	server_session_ssl::server_session_ssl(service& srv, std::shared_ptr<tcp::acceptor> acceptor, std::shared_ptr<encryption> encr)
		: _encryption(encr), _socket(new ssl_socket(srv, encr->context())), _acceptor(acceptor)
	{
	}

	server_session_ssl::~server_session_ssl()
	{
	}

	void server_session_ssl::start()
	{
		_socket->set_verify_mode(_encryption->verify_mode());

		auto verify_callback = [this](bool preverified, verify_context& context)
		{
			return _encryption->verify_certificate(preverified, context);
		};

		_socket->set_verify_callback(verify_callback);

		auto handler = std::bind(&server_session_ssl::accept_handler, this, std::placeholders::_1);

		_acceptor->async_accept(_socket->lowest_layer(), handler);
	}

	void server_session_ssl::accept_handler(const error_code& err)
	{
		if (err)
		{
			connected(nullptr, err);

			return;
		}

		auto handler = std::bind(&server_session_ssl::handshake_handler, this, std::placeholders::_1);

		_socket->async_handshake(ssl_stream_base::server, handler);
	}

	void server_session_ssl::handshake_handler(const error_code& err)
	{
		std::shared_ptr<connection> conn;

		if (!err)
		{
			std::shared_ptr<socket> sckt = std::make_shared<socket_ssl>(_socket);

			conn = std::make_shared<connection>(sckt);
		}

		connected(conn, err);
	}
}
