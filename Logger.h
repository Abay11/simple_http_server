#pragma once

#include <string>

static std::string time()
{
	std::string res;
	return res;
}

class Logger
{
public:
	enum LVL {DEB, WARN, ERR};
	
	Logger(std::ostream& os) :
		os_(os),
		lvl_(DEB)
	{}

	Logger(const Logger&) = delete;
	Logger operator=(const Logger&) = delete;

	void SetLevel(LVL level)
	{
		lvl_ = level;
	}

	template <typename T>
	void Debug(const T& t)
	{
		return Debug(t, "");
	}

	template <typename T, typename ... ARGS>
	void Debug(const T& t, ARGS...args)
	{
		if (lvl_ > DEB) return;

		os_ << time() << " DEBUG: " << t << " ";
		return Log(args...);
	}

	template <typename T>
	void Warn(const T& t)
	{
		return Warn(t, "");
	}

	template <typename T, typename ... ARGS>
	void Warn(const T& t, ARGS...args)
	{
		os_ << time() << " WARNING: " << t << " ";
		return Log(args...);
	}
	
	template <typename T>
	void Error(const T& t)
	{
		return Error(t, "");
	}

	template <typename T, typename ... ARGS>
	void Error(const T& t, ARGS...args)
	{
		os_ << time() << " ERROR: " << t << " ";
		return Log(args...);
	}

private:
	template <typename T, typename ... ARGS>
	void Log(const T& t, ARGS...args)
	{
		os_ << t << " ";

		return Log(args...);
	}

	template <typename T>
	void Log(const T& t)
	{
		os_ << t << std::endl;
	}

private:
	static Logger* instance_;

	std::ostream& os_;

	LVL lvl_;
};
