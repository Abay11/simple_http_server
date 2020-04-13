#include "Slave.h"
#include "HttpServer.h"

#include <fstream>

//#include <boost/asio.hpp>

bool validateIndex(const std::string& file_path)
{
	//path should be non empty, absolute
	//and starts with '/'
	if (file_path.empty() ||
		file_path.front() != '/' ||
		file_path.find("..") != std::string::npos)
		return false;

	return true;
}
	
 Slave::Slave(Server* server, ip::tcp::socket&& socket):
		socket_(std::move(socket)),
		log_(server->getLogger()),
	 server_(server)
	{
 }

void Slave::do_read()
{
	using namespace std::placeholders;
	memset(inBuffer_, 0, BUFFER_LENGTH);
	socket_.async_receive(buffer(inBuffer_, BUFFER_LENGTH), std::bind(&Slave::read_handler, this, _1, _2));
}

void Slave::read_handler(const boost::system::error_code & error, size_t bytes)
{
	log_->Debug("Start reading");
	std::string request_str(inBuffer_);
	log_->Debug("Received from a slave: ", request_str);

	if (request_str.empty())
	{
		close_connection();
		return;
	}

	parse_request(request_str, request_);

	boost::asio::post(socket_.get_executor(),
		[this]() {handle_request(&request_);});
}

void Slave::do_write()
{
	log_->Debug("Sending a reply to a slave");
	using namespace std::placeholders;
	response_.to_string(outBuffer_);
	boost::asio::async_write(socket_,
		boost::asio::buffer(outBuffer_),
		std::bind(&Slave::write_handler, this, _1, _2));
}

void Slave::write_handler(const boost::system::error_code & error, std::size_t bytes_transferred)
{
	if(error)
	{
		log_->Error("Can't send reply");
	}

	log_->Debug("Sent reply:\n", outBuffer_);

	close_connection();
}

void Slave::handle_request(const Request* request)
{
	do
	{
		if (request->method() == METHOD_GET)
		{
			if (!validateIndex(request->index()))
			{
				_notfound:
				response_.setCode(Response::NOT_FOUND);
				break;
			}

			std::stringstream full_path;
			full_path << server_->getRootDir();
			
			full_path << request->index()
				<< (request->index().back() == '/' ? "index.html" : "");

			log_->Debug("Full path of the requested file is: ", full_path.str());

			std::ifstream local_file(full_path.str());
			if (!local_file.is_open())
			{
				goto _notfound;
			}			

			std::string in;
			while (std::getline(local_file, in))
			{
				response_.content().append(in);
}

			response_.setCode(Response::OK);
		}
		else
		{
			response_.setCode(Response::SERVER_ERROR);
		}
	} while (0);
	
	boost::asio::post(socket_.get_executor(),
		[this]() {do_write();});
}

void Slave::close_connection()
{
	server_->CloseSession(shared_from_this());
}

