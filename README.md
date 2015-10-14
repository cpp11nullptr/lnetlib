# lnetlib: C++ network library

lnetlib (or lightweight network library) is a small C++ network library with
simple API for quick development network applications. This library is based on non-Boost
Asio and has written on modern C++11.

lnetlib has next features:
* multithreading server
* stream abstraction over network
* request/response model
* optional encryption

### Internal dependencies

* Asio
* lsignal

### Introduction

| Directory          | Description                              |
|--------------------|------------------------------------------|
| `lnetlib/`         | Core files of library                    |
| `lnetlib/asio/`    | Network subsystem                        |
| `lnetlib/lsignal/` | Signal/slot subsystem                    |
| `example/`         | Example of using lnetlib                 |
| `example/client/`  | Client application                       |
| `example/server/`  | Server application                       |

### How to use

Just copy `lnetlib` directory in your project or create standalone static/dynamic library from it. 

Library divided into two core classes - server and client. First of all, create instance of
client or server class and connect your defined slots to necessary signals. If you want to
use encryption you should init encryption options before client or server will be started.
Also, you can setup desired thread policy (for server only). Finally, start listening (for server)
or connect to server (for client).

### Essential classes

##### connection

This class handle active connection between client and server. Although signals of this class
routing through client/server class you can directly connect to them:

| Signal                | Description                                          |
|-----------------------|------------------------------------------------------|
| _closed_              | Emit if connection is closed                         |
| _error_               | Emit if error is raised                              |
| _received_            | Emit if input data available                         |

Method _create_stream_ creates output network stream for target command and can accept response
callback which will be called if you create response of this stream/command on the other peer.

##### ostream

Subclass of std::ostream. This is representation of output network stream. Method _send_ sends
data.

##### istream

Subclass of std::istream. This is representation of input network stream. Method _create_response_
creates response output stream (see description of _connection_ class).

##### encryption

Through this class you can set necessary encryption options:

| Option                | Description                                          |
|-----------------------|------------------------------------------------------|
| _encryption method_   | Enable appropriate SSL protocol (SSLv2, SSLv2, etc)  |
| _CA_                  | Set certificate authority                            |
| _certificate_         | Set PEM certificate                                  |
| _private key_         | Set PEM private key                                  |
| _verify mode_         | Enable peer verify mode (see ssl_verify_mode)        |
| _verify callback_     | Set callback for verificaton peer certificate        |

##### server

Server supports two thread policies:

| Thread policy         | Description                                          |
|-----------------------|------------------------------------------------------|
| _one_per_connection_  | Every new connection creates new thread              |
| _fixed_count_         | Fixed thread count                                   |

Although lnetlib uses only one i/o service, which is shared between server threads, all network
operations are asynchonous.

This class contains such signals:

| Signal                | Description                                          |
|-----------------------|------------------------------------------------------|
| _connected_           | Emit if new connection is established                |
| _disconnected_        | Emit if connection is closed                         |
| _error_               | Emit if error is raised                              |
| _received_            | Emit if input data available                         |

##### client

This class contains such signals:

| Signal                | Description                                          |
|-----------------------|------------------------------------------------------|
| _refused_             | Emit if server refused connection                    |
| _connected_           | Emit if connection is established                    |
| _disconnected_        | Emit if connection is closed                         |
| _error_               | Emit if error is raised                              |
| _received_            | Emit if input data available                         |

### Example

This console application illustrates features of library lnetlib.
