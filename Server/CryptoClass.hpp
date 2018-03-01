#include <iostream>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <fstream>

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
  char  c[256];
};

class CryptoClass
{
  private:
  const unsigned char *publicKey;
  const unsigned char *privateKey;
  int                 padding;
  int len;

  public:
    CryptoClass();
    ~CryptoClass(){};
    std::string getFileKey(std::string);
    RSA         *CreateRSA(const unsigned char *key, int pub);
  unsigned char *PublicEncrypt(std::string data);
  unsigned char *PrivateDecrypt(unsigned char*);
    void        PrintError(std::string msg);
    unsigned char *ConvertToUnsigned(std::string str);
};
