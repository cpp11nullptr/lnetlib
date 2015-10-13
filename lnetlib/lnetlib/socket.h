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

#ifndef LNETLIB_SOCKET_H
#define LNETLIB_SOCKET_H

#include "config.h"

#include <functional>

#include "asio/error.hpp"
#include "asio/read.hpp"
#include "asio/write.hpp"
#include "asio/streambuf.hpp"

namespace lnetlib
{
	class socket
	{
	public:
		using error_code = asio::error_code;
		using stream_buffer = asio::streambuf;
		using package_buffer = std::vector<stream_buffer::const_buffers_type>;
		using completion_cond = std::function<std::size_t(const error_code&, std::size_t)>;
		using async_read_handler = std::function<void(const error_code&, std::size_t)>;
		using async_write_handler = std::function<void(const error_code&, std::size_t)>;

		socket();
		virtual ~socket();

		virtual bool is_open() const = 0;
		virtual void close() = 0;

		virtual void async_read(stream_buffer& buffer, completion_cond cond, async_read_handler handler) = 0;
		virtual void async_write(const package_buffer& buffer, async_write_handler handler) = 0;

	};
}

#endif // LNETLIB_SOCKET_H

