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

#ifndef LNETLIB_CLIENT_H
#define LNETLIB_CLIENT_H

#include "config.h"

#include <memory>
#include <string>
#include <thread>

#include "asio/error.hpp"
#include "asio/io_service.hpp"
#include "asio/ip/tcp.hpp"

#include "asio/ssl/context.hpp"

#include "lsignal/lsignal.h"

#include "helper.h"
#include "istream.h"
#include "ostream.h"

#include "encryption.h"
#include "connection.h"

#include "client_session.h"
#include "client_session_ssl.h"
#include "client_session_non_ssl.h"

namespace lnetlib
{
	class client
	{
	public:
		using error_code = asio::error_code;
		using service = asio::io_service;
		using tcp = asio::ip::tcp;
		using socket_base = asio::socket_base;
		using address = asio::ip::address;

		lsignal::signal<void()> refused;
		lsignal::signal<void()> connected;
		lsignal::signal<void()> disconnected;
		lsignal::signal<void(int, const std::string&)> error;
		lsignal::signal<void(std::unique_ptr<istream> stream)> received;

		client();
		~client();

		std::shared_ptr<encryption> encryption_info() const;

		void connect(const std::string& hostname, int port);
		void disconnect();

		std::shared_ptr<connection> conn() const;

	private:
		bool _running;
		service _service;
		tcp::resolver::iterator _endpoint;

		std::shared_ptr<encryption> _encryption;
		std::shared_ptr<connection> _connection;
		std::shared_ptr<client_session> _session;

		void try_connect();

		void create_session();

		void connected_handler(client_session *session, std::shared_ptr<connection> conn, const error_code& err);
		void error_handler(std::shared_ptr<connection> conn, int code, const std::string& message);
		void received_handler(std::shared_ptr<connection> conn, std::unique_ptr<istream> stream);

	};
}

#endif // LNETLIB_CLIENT_H

