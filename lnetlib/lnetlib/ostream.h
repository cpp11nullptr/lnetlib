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

#ifndef LNETLIB_OUT_STREAM_H
#define LNETLIB_OUT_STREAM_H

#include "config.h"

#include <memory>
#include <ostream>

#include "asio/streambuf.hpp"

#include "lsignal/lsignal.h"

namespace lnetlib
{
	class ostream
		: public std::ostream
	{	
	public:
		using stream_buffer = asio::streambuf;

		lsignal::signal<void(std::unique_ptr<stream_buffer>)> dispatch;

		template<typename T>
		ostream(T command, uint64_t uid);

		~ostream();

		uint64_t uid() const;

		void send();

	private:
		std::unique_ptr<stream_buffer> _buffer;
		bool _sended;
		uint64_t _uid;

	};

	template<typename T>
	ostream::ostream(T command, uint64_t uid)
		: _buffer(new stream_buffer()), _sended(false), _uid(uid)
	{
		init(_buffer.get());

		uint64_t cmd = static_cast<uint64_t>(command);

		this->write(reinterpret_cast<const char*>(&cmd), sizeof(uint64_t));
		this->write(reinterpret_cast<const char*>(&uid), sizeof(uint64_t));
	}
}

#endif // LNETLIB_OUT_STREAM_H

