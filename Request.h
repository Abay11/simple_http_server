#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string/regex.hpp>
#include <boost/xpressive/xpressive.hpp>

const std::string CRLF = "\\r\\n";

#define METHOD_GET "GET"
#define METHOD_POST "POST"

class Request
{

public:

	void setMethod(const std::string m)
	{
		method_ = m;
	}

	void setIndex(const std::string i)
	{
		index_ = i;
	}

	void setVersion(const std::string v)
	{
		version_ = v;
	}

	std::string method() const { return method_; }
	std::string index() const { return index_; }
	std::string version() const { return version_; }

private:
	std::string method_;
	std::string index_;
	std::string version_;
};


inline void parse_request(const std::string& request, Request& holder)
{
	if (request.empty())
	{
		return;
	}

	using namespace boost::algorithm;
	using lines_t = std::vector<std::string>;
	lines_t lines;
	boost::algorithm::split_regex(lines, request, boost::regex(CRLF));

	using namespace boost::xpressive;
	mark_tag method(1);
	mark_tag index(2);
	mark_tag version(3);
	smatch what;
	sregex rx = (method = +_) >> _s >>
		(index = +_) >> _s >>
		(version = +_);

	if (regex_search(lines[0], what, rx))
	{
		holder.setMethod(what[method]);
		holder.setIndex(what[index]);
		holder.setVersion(what[version]);
	}
}

