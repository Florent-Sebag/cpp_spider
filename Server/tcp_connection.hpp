//
// tcp_connection.hpp for connection.hpp in /home/benamar/cpp_spider/Server
// 
// Made by benamar
// Login   <mahdi.benamar@epitech.eu>
// 
// Started on  Fri Sep 22 17:34:24 2017 benamar
// Last update Sun Oct  1 21:23:16 2017 benamar
//

#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <cstring>
#include <list>
#include <time.h>
#include <typeinfo>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
//#include "CryptoClass.hpp"
#include "DataManagement.hpp"


using boost::asio::ip::tcp;

class Client : public boost::enable_shared_from_this<Client>
{
public:
  
  typedef boost::shared_ptr<Client> pointer;
  static pointer create(boost::asio::io_service& ios) // (1)
  {
    return pointer(new Client(ios));
  }
  tcp::socket& socket();
  void start();
  void  do_read();
  ~Client()
  {
    close();
  };
private:
  
  Client(boost::asio::io_service& io_service);
  void				handle_read_until(const boost::system::error_code& error, size_t);
  void				first_read(const boost::system::error_code& error);
  void				Send_to_DB();
  void				decrypt_to_log();
  void				write_user_log();
  void				close();
  void				Protocole(std::string protocole);
  std::string			get_id();
  
private:

  Data				data;
  tcp::socket			m_socket;
  std::string			m_message;
  unsigned char			msg[sizeof(Data)];
  bool				identificate;
  std::string			id;
  boost::asio::streambuf	resp;
  DataManagement		DB;
};


#endif
