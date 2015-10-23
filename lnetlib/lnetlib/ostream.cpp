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

#include "ostream.h"

namespace lnetlib
{
	ostream::~ostream()
	{
		send();
	}

	uint64_t ostream::uid() const
	{
		return _uid;
	}

	void ostream::write_int8(int8_t data)
	{
		write_basic<int8_t>(data);
	}

	void ostream::write_uint8(uint8_t data)
	{
		write_basic<uint8_t>(data);
	}

	void ostream::write_int16(int16_t data)
	{
		write_basic<int16_t>(data);
	}

	void ostream::write_uint16(uint16_t data)
	{
		write_basic<uint16_t>(data);
	}

	void ostream::write_int32(int32_t data)
	{
		write_basic<int32_t>(data);
	}

	void ostream::write_uint32(uint32_t data)
	{
		write_basic<uint32_t>(data);
	}

	void ostream::write_int64(int64_t data)
	{
		write_basic<int64_t>(data);
	}

	void ostream::write_uint64(uint64_t data)
	{
		write_basic<uint64_t>(data);
	}

	void ostream::write_float32(float data)
	{
		write_basic<float>(data);
	}

	void ostream::write_float64(double data)
	{
		write_basic<double>(data);
	}

	void ostream::send()
	{
		if (_sended)
		{
			return;
		}

		_sended = true;

		dispatch(std::move(_buffer));
	}
}
