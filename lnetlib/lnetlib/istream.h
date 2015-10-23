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

		int8_t read_int8();
		uint8_t read_uint8();

		int16_t read_int16();
		uint16_t read_uint16();

		int32_t read_int32();
		uint32_t read_uint32();

		int64_t read_int64();
		uint64_t read_uint64();

		float read_float32();
		double read_float64();

		template<typename T = uint64_t>
		std::string read_string();

		template<typename T = uint64_t>
		const char* read_data_chunk(T& size);

		template<typename T = uint64_t>
		std::vector<char> read_data_chunk();

		std::unique_ptr<ostream> create_response();

	private:
		std::shared_ptr<stream_buffer> _buffer;
		uint64_t _uid;
		uint64_t _command;

		template<typename T>
		T read_basic();

	};

	template<typename T>
	T istream::command() const
	{
		return static_cast<T>(_command);
	}

	template<typename T>
	std::string istream::read_string()
	{
		T size = read_basic<T>();

		char data[size];
		read(data, size);

		return std::string(data, 0, size);
	}

	template<typename T>
	const char* istream::read_data_chunk(T& size)
	{
		size = read_basic<T>();

		char *data = new char[size];
		read(data, size);

		return data;
	}

	template<typename T>
	std::vector<char> istream::read_data_chunk()
	{
		T size = read_basic<T>();

		std::vector<char> chunk(size);
		read(chunk.data(), chunk.size());

		return chunk;
	}

	template<typename T>
	T istream::read_basic()
	{
		T data;

		read(reinterpret_cast<char*>(&data), sizeof(T));

		return data;
	}
}

#endif // LNETLIB_ISTREAM_H

