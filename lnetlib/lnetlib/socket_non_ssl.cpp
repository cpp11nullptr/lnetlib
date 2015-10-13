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

#include "socket_non_ssl.h"

namespace lnetlib
{
	socket_non_ssl::socket_non_ssl(std::shared_ptr<tcp::socket> socket)
		: _socket(socket)
	{
	}

	socket_non_ssl::~socket_non_ssl()
	{
	}

	bool socket_non_ssl::is_open() const
	{
		return _socket->is_open();
	}

	void socket_non_ssl::close()
	{
		_socket->shutdown(socket_base::shutdown_both);
		_socket->close();
	}

	void socket_non_ssl::async_read(stream_buffer& buffer, completion_cond cond, async_read_handler handler)
	{
		asio::async_read(*_socket, buffer, cond, handler);
	}

	void socket_non_ssl::async_write(const package_buffer& buffer, async_write_handler handler)
	{
		asio::async_write(*_socket, buffer, handler);
	}
}
