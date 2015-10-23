#include <lnetlib/client.h>
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

	lnetlib::client client;

	client.refused.connect([]()
	{
		std::cout << "connection refused\n";
		std::cout.flush();
	});

	client.connected.connect([]()
	{
		std::cout << "connected\n";
		std::cout.flush();
	});

	client.disconnected.connect([]()
	{
		std::cout << "disconnected\n";
		std::cout.flush();
	});

	client.error.connect([](int code, const std::string& message)
	{
		std::cout << "error (" << code << "): " << message << "\n";
		std::cout.flush();
	});

	client.received.connect([](std::unique_ptr<lnetlib::istream> stream)
	{
		switch (stream->command<Command>())
		{
		case Command::Hello:
			{
				std::string msg = stream->read_string();

				std::cout << "server says: hello, " << msg << "\n";
				std::cout.flush();

				break;
			}

		case Command::Goodbye:
			{
				std::cout << "server says: goodbye!\n";
				std::cout.flush();

				break;
			}

		case Command::SendFile:
			{
				std::string filename = stream->read_string();
				std::vector<char> bytes = stream->read_data_chunk();

				std::ofstream fstream(filename, std::ios::binary | std::ios::out);
				fstream.write(bytes.data(), bytes.size());

				std::cout << "received file \"" << filename << "\"\n";
				std::cout.flush();

				break;
			}

		default:
			break;
		}
	});

	auto encryption = client.encryption_info();

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
	encryption->set_cert_filename("client.crt");
	encryption->set_private_key_filename("client.key");
	encryption->set_verify_mode(lnetlib::ssl_verify_peer);
	encryption->set_enabled(true);

	client.connect("127.0.0.1", 7777);

	char cmd = 0;
	bool loop = true;

	while (loop)
	{
		std::cin >> cmd;

		switch (cmd)
		{
		case 'q':
			{
				auto stream = client.conn()->create_stream(Command::Quit);
				stream->send();

				break;
			}

		case 'm':
			{
				auto callback = [](std::unique_ptr<lnetlib::istream> stream)
				{
					std::string msg = stream->read_string();

					std::cout << "callback message: " << msg << "\n";
					std::cout.flush();
				};

				auto stream = client.conn()->create_stream(Command::Message, callback);
				stream->send();

				break;
			}

		default:
			break;
		}
	}

	return 0;
}
