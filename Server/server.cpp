//
// server.cpp for server in /home/benamar/cpp_spider/Server
// 
// Made by benamar
// Login   <mahdi.benamar@epitech.eu>
// 
// Started on  Thu Sep 21 20:55:15 2017 benamar
// Last update Sun Oct  1 20:52:02 2017 benamar
//
//

#include "tcp_server.hpp"

tcp_server::tcp_server(boost::asio::io_service& io_service, int port) : m_acceptor(io_service, tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
  start_accept();
}

void	tcp_server::start_accept()
{
  Client::pointer new_connection = Client::create(m_acceptor.get_io_service());
  
  m_acceptor.async_accept(new_connection->socket(),
			  boost::bind(&tcp_server::handle_accept, this,
			  new_connection,
			  boost::asio::placeholders::error));
}

void	tcp_server::handle_accept(Client::pointer new_connection, const boost::system::error_code& error)
{
  if (!error)
    {
      std::cout << "Recu un client" << std::endl;
      new_connection->start();
      start_accept();
    }
}

int main()
{
  try
    {
      boost::asio::io_service io_service;
      
      tcp_server server(io_service, 7272);
      io_service.run();
    }
  catch (std::exception& e)
    {
      std::cout << "erreur mon pote\n";
      std::cerr << e.what() << std::endl;
    }
  return 0;
}
