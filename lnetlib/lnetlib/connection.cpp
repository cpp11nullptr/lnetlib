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

#include "connection.h"

namespace lnetlib
{
	connection::connection(std::shared_ptr<socket> sckt)
		: _socket(sckt), _socket_locked(false), _uid_counter(0)
	{
		wait_package();
	}

	connection::~connection()
	{
	}

	bool connection::is_open() const
	{
		return _socket->is_open();
	}

	void connection::close()
	{
		_socket->close();

		closed(shared_from_this());
	}

	bool connection::check_error(const error_code& err)
	{
		if (!err)
		{
			return true;
		}

		switch (err.value())
		{
		case asio::error::operation_aborted:
			break;

		case custom_error::no_such_file_or_directory:
			closed(shared_from_this());
			break;

		case custom_error::ssl_short_read:
			closed(shared_from_this());
			break;

		default:
			error(shared_from_this(), err.value(), err.message());
			break;
		}

		return false;
	}

	void connection::wait_package()
	{
		std::lock_guard<std::mutex> locker(_mutex);

		std::shared_ptr<stream_buffer> buffer = std::make_shared<stream_buffer>();

		auto handler = std::bind(&connection::read_package_size_handler, this, buffer, std::placeholders::_1, std::placeholders::_2);

		_socket->async_read(*buffer.get(), asio::transfer_exactly(sizeof(uint64_t)), handler);
	}

	void connection::wait_package_body(uint64_t size)
	{
		std::lock_guard<std::mutex> locker(_mutex);

		std::shared_ptr<stream_buffer> buffer = std::make_shared<stream_buffer>();

		auto handler = std::bind(&connection::read_package_body_handler, this, buffer, std::placeholders::_1, std::placeholders::_2);

		_socket->async_read(*buffer.get(), asio::transfer_exactly(size), handler);
	}

	void connection::read_package_size_handler(std::shared_ptr<stream_buffer> buffer, const error_code& err, std::size_t bytes)
	{
		if (!check_error(err))
		{
			return;
		}

		if (bytes == 0)
		{
			return;
		}

		uint64_t size = 0;

		std::istream stream(buffer.get());
		stream.read(reinterpret_cast<char*>(&size), sizeof(uint64_t));

		wait_package_body(size);
	}

	void connection::read_package_body_handler(std::shared_ptr<stream_buffer> buffer, const error_code& err, std::size_t bytes)
	{
		if (!check_error(err))
		{
			return;
		}

		if (bytes == 0)
		{
			return;
		}

		std::unique_ptr<istream> stream { new istream(buffer) };

		stream->response_dispatch.connect(create_dispatch_slot());

		auto iter = _callbacks.find(stream->uid());

		if (iter != _callbacks.end())
		{
			callback cb = _callbacks[stream->uid()];

			_callbacks.erase(iter);

			cb(std::move(stream));
		}
		else
		{
			received(shared_from_this(), std::move(stream));
		}

		wait_package();
	}

	std::shared_ptr<connection::package> connection::create_package(std::unique_ptr<stream_buffer> buffer) const
	{
		std::unique_ptr<stream_buffer> size_buffer { new stream_buffer() };
		std::ostream size_stream(size_buffer.get());

		uint64_t size = asio::buffer_size(buffer->data());
		size_stream.write(reinterpret_cast<char*>(&size), sizeof(uint64_t));

		std::shared_ptr<package> pkg = std::make_shared<package>();

		pkg->size_buffer = std::move(size_buffer);
		pkg->data_buffer = std::move(buffer);

		pkg->buffer = std::unique_ptr<package_buffer>(new package_buffer());

		pkg->buffer->push_back(pkg->size_buffer->data());
		pkg->buffer->push_back(pkg->data_buffer->data());

		return pkg;
	}

	void connection::send_package(std::shared_ptr<package> pkg)
	{
		auto write_handler = [this](std::shared_ptr<package> pkg, const error_code& err, std::size_t bytes)
		{
			(void)pkg;
			(void)bytes;

			if (!check_error(err))
			{
				return;
			}

			std::lock_guard<std::mutex> locker(_mutex);

			if (!_packages.empty())
			{
				std::shared_ptr<package> pkg = _packages.front();

				_packages.pop();

				send_package(pkg);
			}
			else
			{
				_socket_locked = false;
			}
		};

		auto handler = std::bind(write_handler, pkg, std::placeholders::_1, std::placeholders::_2);

		_socket_locked = true;

		_socket->async_write(*(pkg->buffer.get()), handler);
	}

	connection::dispatch_slot connection::create_dispatch_slot()
	{
		dispatch_slot slot = [this](std::unique_ptr<stream_buffer> buffer)
		{
			std::shared_ptr<package> pkg = create_package(std::move(buffer));

			std::lock_guard<std::mutex> locker(_mutex);

			if (_socket_locked)
			{
				_packages.push(pkg);
			}
			else
			{
				send_package(pkg);
			}
		};

		return slot;
	}
}
