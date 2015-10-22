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

#include "server.h"

namespace lnetlib
{
	server::server()
		: _running(false), _thread_policy(server_thread_policy::one_per_connection),
		  _thread_count(0), _encryption(new encryption())
	{
	}

	server::~server()
	{
	}

	server_thread_policy server::thread_policy() const
	{
		return _thread_policy;
	}

	void server::set_thread_policy(server_thread_policy thread_policy, int32_t thread_count)
	{
		if (_running)
		{
			return;
		}

		_thread_policy = thread_policy;
		_thread_count = thread_count;
	}

	std::shared_ptr<encryption> server::encryption_info() const
	{
		return _encryption;
	}

	void server::start(const std::string& addr, int port)
	{
		if (_running)
		{
			return;
		}

		if (!_encryption->last_error().empty())
		{
			error(nullptr, -1, _encryption->last_error());

			return;
		}

		_endpoint = tcp::endpoint(address::from_string(addr), port);

		try
		{
			_acceptor = std::make_shared<tcp::acceptor>(_service, _endpoint);

			_running = true;

			wait_first_connection();
		}
		catch (const asio::system_error& sys_err)
		{
			error(nullptr, sys_err.code().value(), sys_err.code().message());
		}
	}

	void server::stop()
	{
		if (!_running)
		{
			return;
		}

		_service.stop();

		_running = false;
	}

	std::list<std::shared_ptr<connection>> server::connections()
	{
		std::lock_guard<std::mutex> locker(_mutex);

		return _connections;
	}

	void server::wait_first_connection()
	{
		create_session();

		switch (_thread_policy)
		{
		case server_thread_policy::fixed_count:
			init_service_threads();
			break;

		case server_thread_policy::one_per_connection:
			create_service_thread();
			break;

		default:
			break;
		}
	}

	void server::wait_next_connection()
	{
		create_session();

		if (_thread_policy == server_thread_policy::one_per_connection)
		{
			create_service_thread();
		}
	}

	void server::create_session()
	{
		server_session *session = nullptr;

		if (!_encryption->is_enabled())
		{
			session = new server_session_non_ssl(_service, _acceptor);
		}
		else
		{
			session = new server_session_ssl(_service, _acceptor, _encryption);
		}

		auto handler = std::bind(&server::connected_handler, this, session, std::placeholders::_1, std::placeholders::_2);

		session->connected.connect(handler);

		session->start();
	}

	void server::connected_handler(server_session *session, std::shared_ptr<connection> conn, const error_code& err)
	{
		if (err)
		{
			error(conn, err.value(), err.message());
		}
		else
		{
			add_connection(conn, std::shared_ptr<server_session>(session));

			wait_next_connection();
		}
	}

	void server::add_connection(std::shared_ptr<connection> conn, std::shared_ptr<server_session> session)
	{
		auto closed_slot = [this](std::shared_ptr<connection> conn)
		{
			disconnected(conn);

			remove_connection(conn);
		};

		auto error_slot = [this](std::shared_ptr<connection> conn, int code, const std::string& message)
		{
			error(conn, code, message);
		};

		auto received_slot = [this](std::shared_ptr<connection> conn, std::unique_ptr<istream> stream)
		{
			received(conn, std::move(stream));
		};

		conn->closed.connect(closed_slot);
		conn->error.connect(error_slot);
		conn->received.connect(received_slot);

		std::lock_guard<std::mutex> locker(_mutex);

		_connections.push_back(conn);

		_sessions[conn] = session;

		connected(conn);
	}

	void server::remove_connection(std::shared_ptr<connection> conn)
	{
		std::lock_guard<std::mutex> locker(_mutex);

		auto connection_iter = std::find(_connections.begin(), _connections.end(), conn);

		if (connection_iter != _connections.end())
		{
			_connections.erase(connection_iter);
		}

		auto session_iter = _sessions.find(conn);

		if (session_iter != _sessions.end())
		{
			_sessions.erase(session_iter);
		}
	}

	void server::init_service_threads()
	{
		if (_thread_count == -1)
		{
			_thread_count = std::thread::hardware_concurrency();

			if (_thread_count == -1)
			{
				_thread_count = 2;
			}
		}

		if (_thread_count > 0)
		{
			for (int i = 0; i < _thread_count; ++i)
			{
				create_service_thread();
			}
		}
		else
		{
			_service.run();
		}
	}

	void server::create_service_thread()
	{
		auto thread_func = [this]()
		{
			_service.run();
		};

		std::thread thread(thread_func);

		thread.detach();
	}
}
