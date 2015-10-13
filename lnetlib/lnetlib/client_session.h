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

#ifndef LNETLIB_CLIENT_SESSION_H
#define LNETLIB_CLIENT_SESSION_H

#include "config.h"

#include "asio/error.hpp"
#include "asio/io_service.hpp"
#include "asio/connect.hpp"

#include "lsignal/lsignal.h"

#include "connection.h"

namespace lnetlib
{
	class client_session
	{
	public:
		using error_code = asio::error_code;
		using service = asio::io_service;
		using tcp = asio::ip::tcp;

		lsignal::signal<void(std::shared_ptr<connection>, const error_code&)> connected;

		client_session();
		virtual ~client_session();

		virtual void start() = 0;

	};
}

#endif // LNETLIB_CLIENT_SESSION_H

