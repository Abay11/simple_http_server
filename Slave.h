#pragma once
/*
This class is presented connected HTTP clients.
*/
#include <boost/asio/ip/tcp.hpp>

#include <array>

#include "Logger.h"
#include "Request.h"
#include "Response.h"

namespace
{
	using namespace boost::asio;
}

class Server;

class Slave : public std::enable_shared_from_this<Slave>
{
public:

	explicit Slave(Server* server, ip::tcp::socket&& socket);

	void do_read();

	//use port used identificator
	unsigned short getSlaveID()
	{
		if (socket_.is_open())
			return socket_.remote_endpoint().port();

		return 0;
	}

private:
	void read_handler(const boost::system::error_code& error,
		size_t bytes);

	void do_write();

	void write_handler(const boost::system::error_code& error,
		std::size_t bytes_transferred);

	void handle_request(const Request* request);

	void close_connection();

private:
	enum
	{
		BUFFER_LENGTH = 1450
	};

	char inBuffer_[BUFFER_LENGTH];
	std::string outBuffer_;

	ip::tcp::socket socket_;

	Logger* log_;

	Server* server_ = nullptr;

	Request request_;
	Response response_;
};

