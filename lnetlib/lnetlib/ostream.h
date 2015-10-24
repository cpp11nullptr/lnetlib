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

		void write_int8(int8_t data);
		void write_uint8(uint8_t data);

		void write_int16(int16_t data);
		void write_uint16(uint16_t data);

		void write_int32(int32_t data);
		void write_uint32(uint32_t data);

		void write_int64(int64_t data);
		void write_uint64(uint64_t data);

		void write_float32(float data);
		void write_float64(double data);

		template<typename T = uint64_t>
		void write_string(const std::string& data);

		template<typename T = char, typename U = uint64_t>
		void write_data_chunk(const T *data, U size);

		template<typename T = char, typename U = uint64_t>
		void write_data_chunk(const std::vector<T>& data);

		void send();

	private:
		std::unique_ptr<stream_buffer> _buffer;
		bool _sended;
		uint64_t _uid;

		template<typename T>
		void write_basic(T data);

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

	template<typename T>
	void ostream::write_basic(T data)
	{
		write(reinterpret_cast<const char*>(&data), sizeof(T));
	}

	template<typename T>
	void ostream::write_string(const std::string& data)
	{
		write_basic<T>(static_cast<T>(data.size()));
		write(data.data(), data.size());
	}

	template<typename T, typename U>
	void ostream::write_data_chunk(const T *data, U size)
	{
		write_basic<U>(size);
		write(reinterpret_cast<const char*>(data), size * sizeof(T));
	}

	template<typename T, typename U>
	void ostream::write_data_chunk(const std::vector<T>& data)
	{
		write_data_chunk(reinterpret_cast<const char*>(data.data()), static_cast<U>(data.size()));
	}
}

#endif // LNETLIB_OUT_STREAM_H

