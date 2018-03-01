//
// tcp_server.hpp for tcp_server.hpp in /home/benamar/cpp_spider/Server
// 
// Made by benamar
// Login   <mahdi.benamar@epitech.eu>
// 
// Started on  Fri Sep 22 19:19:33 2017 benamar
// Last update Sun Oct  1 20:51:58 2017 benamar
//

#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include "tcp_connection.hpp"

using boost::asio::ip::tcp;

class Client;

class tcp_server
{
public:
  
  tcp_server(boost::asio::io_service& io_service, int port);
  
private:
  
  void start_accept();
  void handle_accept(Client::pointer new_connection, const boost::system::error_code& error);
  
private:
  tcp::acceptor m_acceptor;
};

#endif
