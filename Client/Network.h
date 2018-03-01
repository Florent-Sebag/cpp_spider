#pragma once

#ifndef Network_H_
# define Network_H_

#include "boost\asio.hpp"
#include "boost\bind.hpp"
#include "boost\shared_ptr.hpp"
#include "boost/array.hpp"
#include <exception>
#include <iostream>

using boost::asio::ip::tcp;

enum PacketType
{
	KEYSTROKE,
	MOUSE,
	COMMAND,
	RESPONSE
};

typedef struct			s_data
{
	u_int				Timestamp;
	PacketType			Type;
	char				Data[256];
}						t_Data;
class Network
{
private:
	boost::asio::io_service			IOS_;
	boost::asio::ip::tcp::socket	Socket_;
	boost::asio::ip::tcp::endpoint	Endpoint_;
	boost::asio::streambuf			buff;
	bool							readable;

public:
	int								tryConnect(std::string&, int, std::string&);
	int								Disconnect();
	int								SendData(std::string&);
	int								SendData(unsigned char*);
	std::string 					RcvData();
	void							handler(const boost::system::error_code&);

public:
	Network();
	~Network();
};

#endif // !Network_H_
