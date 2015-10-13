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

#ifndef LNETLIB_ISTREAM_H
#define LNETLIB_ISTREAM_H

#include "config.h"

#include <istream>
#include <memory>

#include "asio/streambuf.hpp"

#include "ostream.h"

namespace lnetlib
{
	class istream
		: public std::istream
	{
	public:
		using stream_buffer = asio::streambuf;

		lsignal::signal<void(std::unique_ptr<stream_buffer>)> response_dispatch;

		istream(std::shared_ptr<stream_buffer> buffer);
		~istream();

		template<typename T>
		T command() const;

		uint64_t uid() const;

		std::unique_ptr<ostream> create_response();

	private:
		std::shared_ptr<stream_buffer> _buffer;
		uint64_t _uid;
		uint64_t _command;

	};

	template<typename T>
	T istream::command() const
	{
		return static_cast<T>(_command);
	}
}

#endif // LNETLIB_ISTREAM_H

