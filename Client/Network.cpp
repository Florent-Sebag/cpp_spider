#include "Network.h"
#include <fstream>
#include <string>

void				Network::handler(const boost::system::error_code& ec)
{
	this->readable = true;
	std::cout << "handler" << std::endl;
}

int					Network::tryConnect(std::string& ip, int port, std::string& id)
{
	try {
		this->Endpoint_ = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port);
		this->Socket_.connect(this->Endpoint_);
	}
	catch (const boost::system::system_error& e) {
		(void)e;
		std::cout << "Unable to connect : " << e.what() << std::endl;
		return (-1);
	}
	return (0);
}

int					Network::SendData(std::string& msg)
{
	boost::system::error_code error;
	this->Socket_.write_some(boost::asio::buffer(msg.c_str(), msg.size()), error);
	if (error)
		return (-1);
	return (0);
}

int					Network::SendData(unsigned char* msg)
{
	boost::system::error_code error;
	this->Socket_.write_some(boost::asio::buffer(msg, sizeof(t_Data)), error);
	if (error)
		return (-1);
	return (0);
}

std::string			Network::RcvData()
{
	std::string a;
	boost::asio::read_until(this->Socket_, this->buff, "\0");
	std::string s((std::istreambuf_iterator<char>(&this->buff)), std::istreambuf_iterator<char>());
	return (s);
}

int					Network::Disconnect()
{
	this->Socket_.close();
	return (0);
}
Network::Network() : Socket_(this->IOS_)
{
	this->readable = false;
}


Network::~Network()
{
}
