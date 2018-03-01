#ifndef PSWD_H_
# define PSWD_H_

#include <termios.h>
#include <unistd.h>
#include <iostream>

class Pswd
{
  protected:
    std::string password;
  public:
    Pswd(){};
    ~Pswd(){};
    std::string getPswd();
    std::string ScanPswd();
    void        setStdinEcho(bool);
};

#endif
