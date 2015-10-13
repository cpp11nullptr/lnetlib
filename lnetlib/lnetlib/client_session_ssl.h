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

#ifndef LNETLIB_CLIENT_SESSION_SSL_H
#define LNETLIB_CLIENT_SESSION_SSL_H

#include "config.h"

#include "asio/ssl/stream.hpp"
#include "asio/ssl/verify_context.hpp"

#include "client_session.h"
#include "encryption.h"
#include "socket_ssl.h"

namespace lnetlib
{
	class client_session_ssl
		: public client_session
	{
	public:
		using ssl_socket = asio::ssl::stream<tcp::socket>;
		using ssl_stream_base = asio::ssl::stream_base;
		using verify_context = asio::ssl::verify_context;

		client_session_ssl(service& srv, tcp::resolver::iterator endpoint, std::shared_ptr<encryption> encr);
		~client_session_ssl() override;

		void start() override;

	private:
		std::shared_ptr<encryption> _encryption;
		std::shared_ptr<ssl_socket> _socket;
		tcp::resolver::iterator _endpoint;

		void connect_handler(const error_code& err);
		void handshake_handler(const error_code& err);

	};

}

#endif // LNETLIB_CLIENT_SESSION_SSL_H

