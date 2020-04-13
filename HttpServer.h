#pragma once

#include <string>
#include <iostream>

#include <boost/asio.hpp>

#include "Logger.h"

class Slave;

using namespace boost::asio;

class Server
{
public:

	Server(const std::string& dir,
		const std::string& ip,
		const std::string& port,
		Logger* logger = nullptr);

	void Start();

	void Stop();

	void CloseSession(std::shared_ptr<Slave> slave);

	Logger* getLogger() { return log_; }

	std::string getRootDir()
	{
		return dir_;
	}

private:
	void do_accept();
	void accept_handler(boost::system::error_code ec, ip::tcp::socket s);

private:
	Logger* log_;
	std::string dir_;
	std::string ip_;
	std::string port_;

	io_context io_ctx_;
	std::shared_ptr<io_context::work> work_;
	ip::tcp::acceptor acceptor_;

	std::vector<std::shared_ptr<Slave>> slaves;
};
