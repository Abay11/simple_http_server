#include "HttpServer.h"
#include "Slave.h"
	
Server::Server(const std::string& dir,
	const std::string& ip,
	const std::string& port,
	Logger* logger) :
	dir_(dir),
	ip_(ip),
	port_(port),
	work_(new boost::asio::io_context::work(io_ctx_)),
	acceptor_(io_ctx_)
{
	if (dir_.empty())
		dir_ = ".";
	
	if (logger)
		log_ = logger;
	else
		log_ = new Logger(std::cout);

	log_->Debug("Server params:",
		"root directory:", dir,
		"host ip:", ip_,
		"port:", port_);
}
	
void Server::Start()
{
		log_->Debug("Server started...");

		using namespace boost::asio;
		ip::tcp::acceptor acceptor(io_ctx_);
		ip::tcp::resolver resolver(io_ctx_);
		
		ip::tcp::endpoint ep;
		try
		{

			 ep = *resolver.resolve(ip_, port_);
		}
		catch (const boost::system::error_code& ec)
		{
			log_->Error("Can't resolve address! Please check and restart Server!");
			return;
		}

		acceptor_.open(ep.protocol());
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.bind(ep);
		boost::system::error_code ec;
		acceptor_.listen(32, ec);
		if (ec)
		{
			log_->Error("Can't start server!");
		}

		do_accept();

		io_ctx_.run();
}
	
void Server::Stop()
	{
		work_.reset();
		log_->Debug("Server stopped...");
	}

void Server::CloseSession(std::shared_ptr<Slave> slave)
{
	log_->Debug("Closing connection for a slave", slave->getSlaveID());
	slaves.erase(std::find(slaves.begin(), slaves.end(), slave));
}

void Server::do_accept()
{
		using namespace std::placeholders;
		acceptor_.async_accept(std::bind(&Server::accept_handler, this, _1, _2));
}

void Server::accept_handler(boost::system::error_code ec, ip::tcp::socket s)
{
	if (ec)
	{
		log_->Error("Accepting a new slave is failed");
		return;
	}

	using namespace std::placeholders;
	std::shared_ptr<Slave> slave = std::make_shared<Slave>(Slave(this, std::move(s)));
	slaves.push_back(slave);
	io_ctx_.post(std::bind(&Slave::do_read, slave));
	
	log_->Debug("Accepting a new slave", slave->getSlaveID());

	do_accept();
}
