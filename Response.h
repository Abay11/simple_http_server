#pragma once

#include "Slave.h"
#include <string>

static const std::string RESPONSE_OK = 
			"HTTP/1.0 200 OK\r\n"
				"Content-Type: text/html\r\n"; //content length and other info should be generated automatically

static const std::string RESPONSE_NOT_FOUND =
			"HTTP/1.0 404 Not found\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: 0\r\n"
				"\r\n";

static const std::string RESPONSE_SERVER_ERROR =
			"HTTP/1.0 500 Internal Server Error\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: 0\r\n"
				"\r\n";

class Response
{
public:
		enum Code
		{
			OK = 200,
			NOT_FOUND = 404,
			SERVER_ERROR = 500
		};

	void setCode(Code c)
	{
		code_ = c;
	}

	void to_string(std::string& buffer)
	{
		buffer.clear();

		switch (code_)
		{
		case OK:
			buffer.append(RESPONSE_OK);
			buffer += "Content-Length: ";
			buffer += std::to_string(content_.size());
			buffer += "\r\n\r\n";
			buffer.append(content_);
			break;
		case NOT_FOUND:
			buffer.append(RESPONSE_NOT_FOUND);
			break;
		case SERVER_ERROR:
			buffer.append(RESPONSE_SERVER_ERROR);
			break;
		}
	}

	std::string& content()
	{
		return content_;
	}

private:
	Code code_ = NOT_FOUND;
	std::string content_;
};

