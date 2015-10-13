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

#include "socket_ssl.h"

namespace lnetlib
{
	socket_ssl::socket_ssl(std::shared_ptr<ssl_socket> socket)
		: _socket(socket)
	{
	}

	socket_ssl::~socket_ssl()
	{
	}

	bool socket_ssl::is_open() const
	{
		return _socket->lowest_layer().is_open();
	}

	void socket_ssl::close()
	{
		_socket->lowest_layer().shutdown(socket_base::shutdown_receive);
		_socket->lowest_layer().close();
	}

	void socket_ssl::async_read(stream_buffer& buffer, completion_cond cond, async_read_handler handler)
	{
		asio::async_read(*_socket, buffer, cond, handler);
	}

	void socket_ssl::async_write(const package_buffer& buffer, async_write_handler handler)
	{
		asio::async_write(*_socket, buffer, handler);
	}
}
