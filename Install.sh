sudo apt-get install libssl-dev
sudo apt-get install mysql-server
sudo apt-get install libmysqlclient-dev
cd Server
g++ -std=c++11 DataManagement.cpp Pswd.cpp Installation.cpp `mysql_config --cflags --libs`
./a.out
rm -rf a.out
cd ..
make
