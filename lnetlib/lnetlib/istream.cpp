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

#include "istream.h"

namespace lnetlib
{
	istream::istream(std::shared_ptr<stream_buffer> buffer)
		: std::istream(buffer.get()), _buffer(buffer)
	{
		this->read(reinterpret_cast<char*>(&_uid), sizeof(uint64_t));
		this->read(reinterpret_cast<char*>(&_command), sizeof(uint64_t));
	}

	istream::~istream()
	{
	}

	uint64_t istream::uid() const
	{
		return _uid;
	}

	int8_t istream::read_int8()
	{
		return read_basic<int8_t>();
	}

	uint8_t istream::read_uint8()
	{
		return read_basic<uint8_t>();
	}

	int16_t istream::read_int16()
	{
		return read_basic<int16_t>();
	}

	uint16_t istream::read_uint16()
	{
		return read_basic<uint16_t>();
	}

	int32_t istream::read_int32()
	{
		return read_basic<int32_t>();
	}

	uint32_t istream::read_uint32()
	{
		return read_basic<uint32_t>();
	}

	int64_t istream::read_int64()
	{
		return read_basic<int64_t>();
	}

	uint64_t istream::read_uint64()
	{
		return read_basic<uint64_t>();
	}

	float istream::read_float32()
	{
		return read_basic<float>();
	}

	double istream::read_float64()
	{
		return read_basic<double>();
	}

	std::unique_ptr<ostream> istream::create_response()
	{
		std::unique_ptr<ostream> stream { new ostream(_command, _uid) };

		stream->dispatch.connect([this](std::unique_ptr<stream_buffer> buffer)
		{
			response_dispatch(std::move(buffer));
		});

		return stream;
	}
}
