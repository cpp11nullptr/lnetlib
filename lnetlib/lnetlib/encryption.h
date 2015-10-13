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

#ifndef LNETLIB_ENCRYPTION_H
#define LNETLIB_ENCRYPTION_H

#include "config.h"

#include <functional>

#include "asio/ssl/context.hpp"

#include "helper.h"

namespace lnetlib
{
	enum class encryption_method
	{
		sslv2	= asio::ssl::context::sslv2,
		sslv3	= asio::ssl::context::sslv3,
		tlsv1	= asio::ssl::context::tlsv1,
		sslv23	= asio::ssl::context::sslv23,
		tlsv11	= asio::ssl::context::tlsv11,
		tlsv12	= asio::ssl::context::tlsv12
	};

	enum ssl_verify_mode
	{
		ssl_verify_none						= asio::ssl::verify_none,
		ssl_verify_peer						= asio::ssl::verify_peer,
		ssl_verify_fail_if_no_peer_cert		= asio::ssl::verify_fail_if_no_peer_cert,
		ssl_verify_client_once				= asio::ssl::verify_client_once
	};

	class encryption
	{
	public:
		using const_buffer = asio::const_buffer;
		using ssl_context = asio::ssl::context;
		using verify_context = asio::ssl::verify_context;

		using verify_callback = std::function<bool(bool, X509_STORE_CTX*)>;

		encryption();
		~encryption();

		void init(encryption_method method);

		bool is_enabled() const;
		void set_enabled(bool enabled);

		std::string ca_filename() const;
		void set_ca_filename(const std::string& filename);

		std::string cert_filename() const;
		void set_cert_filename(const std::string& filename);

		std::string private_key_filename() const;
		void set_private_key_filename(const std::string& filename);

		int verify_mode() const;
		void set_verify_mode(int mode);

		void set_verify_callback(verify_callback callback);
		bool verify_certificate(bool preverified, verify_context& context);

		ssl_context& context();

		std::string last_error() const;

	private:
		bool _enabled;
		encryption_method _method;
		std::string _ca_filename;
		std::string _cert_filename;
		std::string _private_key_filename;
		int _verify_mode;
		verify_callback _verify_callback;
		ssl_context _context;
		std::string _last_error;

	};
}

#endif // LNETLIB_ENCRYPTION_H

