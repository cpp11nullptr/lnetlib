#include <lnetlib/server.h>
#include <iostream>
#include <fstream>

enum class Command
{
	Hello = 0,
	Goodbye,
	Quit,
	SendFile,
	Message
};

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	lnetlib::server server;

	server.connected.connect([](std::shared_ptr<lnetlib::connection> conn)
	{
		(void)conn;

		std::cout << "client connected\n";
		std::cout.flush();
	});

	server.disconnected.connect([](std::shared_ptr<lnetlib::connection> conn)
	{
		(void)conn;

		std::cout << "client disconnected\n";
		std::cout.flush();
	});

	server.error.connect([](std::shared_ptr<lnetlib::connection> conn, int code, const std::string& message)
	{
		(void)conn;

		std::cout << "error (" << code << "): " << message << "\n";
		std::cout.flush();
	});

	server.received.connect([](std::shared_ptr<lnetlib::connection> conn, std::unique_ptr<lnetlib::istream> stream)
	{
		switch (stream->command<Command>())
		{
		case Command::Message:
			{
				auto response = stream->create_response();
				(*response) << "ready, steady, go!\n";
				response->send();

				break;
			}

		case Command::Quit:
			{
				auto stream = conn->create_stream(Command::Goodbye);
				stream->send();
				conn->close();

				break;
			}

		default:
			break;
		}
	});

	auto encryption = server.encryption_info();

	encryption->init(lnetlib::encryption_method::tlsv11);

	encryption->set_verify_callback([](bool preverified, X509_STORE_CTX *context) -> bool
	{
		char subject_name[256];

		X509* cert = X509_STORE_CTX_get_current_cert(context);
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

		std::cout << "peer certificate subject name: " << subject_name << "\n";
		std::cout.flush();

		return preverified;
	});

	encryption->set_ca_filename("ca.crt");
	encryption->set_cert_filename("server.crt");
	encryption->set_private_key_filename("server.key");
	encryption->set_verify_mode(lnetlib::ssl_verify_peer | lnetlib::ssl_verify_fail_if_no_peer_cert);
	encryption->set_enabled(true);

	server.start("127.0.0.1", 7777);

	char cmd = 0;
	bool loop = true;

	while (loop)
	{
		std::cin >> cmd;

		switch (cmd)
		{
		case 'h':
			for (auto conn : server.connections())
			{
				auto stream = conn->create_stream(Command::Hello);
				(*stream) << "I'm server!\n";
			}

			break;

		case 'f':
			{
				std::cout << "input filename: ";
				std::cout.flush();

				std::string filename;
				std::cin >> filename;

				uint64_t filename_size;
				filename_size = filename.size();

				std::ifstream fstream(filename, std::ios::binary | std::ios::ate);

				if (!fstream.is_open())
				{
					std::cout << "file not found\n";
					std::cout.flush();

					break;
				}

				uint64_t size = fstream.tellg();

				std::vector<char> bytes;
				bytes.resize(size);
				fstream.seekg(0);
				fstream.read(bytes.data(), size);

				for (auto conn : server.connections())
				{
					auto stream = conn->create_stream(Command::SendFile);

					stream->write(reinterpret_cast<const char*>(&filename_size), sizeof(uint64_t));
					stream->write(filename.data(), filename_size);

					stream->write(reinterpret_cast<const char*>(&size), sizeof(uint64_t));
					stream->write(bytes.data(), bytes.size());

					std::cout << "file sended\n";
					std::cout.flush();
				}
			}

			break;

		case 'q':
			loop = false;
			break;

		default:
			break;
		}
	}

	return 0;
}
