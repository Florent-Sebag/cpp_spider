#include "Pswd.hpp"

void Pswd::setStdinEcho(bool enable)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);

    if( !enable )
        tty.c_lflag = ~ECHO;
    else
        tty.c_lflag = ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

std::string Pswd::ScanPswd()
{
    setStdinEcho(false);
    std::cout << "Mot de passe root de votre base de donnée MySql: ";
    std::cin >> password;
    setStdinEcho(true);
    return (password);
}

std::string Pswd::getPswd()
{
  return (password);
}
