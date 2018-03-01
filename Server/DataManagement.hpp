#ifndef DATAMANAGEMENT_H_
# define DATAMANAGEMENT_H_

#include <my_global.h>
#include <mysql.h>
#include "Pswd.hpp"
//#include "CryptoClass.hpp"

enum PacketType
  {
    KEYSTROKE,
    MOUSE,
    COMMAND,
    RESPONSE,
              AUTH
  };

struct Data
{
  unsigned int  Timestamp;
  PacketType    Type;
  char  Data[256];
};

class DataManagement
{
  protected:
    MYSQL         *sqlObj;
    std::string   nameDB;
    Pswd          PObj;

  public:
    DataManagement();
    ~DataManagement(){};

    int   CreateDB(const std::string &);
  void  CloseConnection();
    int   Query(const std::string &);
    int   CreateUserDb(const std::string &, const std::string &, const std::string &);
    int   Connect(const std::string &, const std::string &, const char *);
    void   CreateTable(const std::string &, const std::string &);
    void   AddDataToTable(const std::string &, const std::string &);
    int   Installation();
    int   InitTable();
    int  AddMessage(std::string from, std::string id, Data msg);
    int  NewMessage(std::string from, Data msg);
    std::string  NewUser(std::string hid);
    MYSQL_RES *retQuery(const std::string &query);
};

#endif
