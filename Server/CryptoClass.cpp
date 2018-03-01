#include "CryptoClass.hpp"
//#include "tcp_connection.hpp"

std::string CryptoClass::getFileKey(std::string path)
{
  std::string   line;
  std::ifstream file(path.c_str());
  std::string   res;

  if (file.is_open())
  {
    while (std::getline(file,line))
      {
        res += line;
        res += '\n';
      }
    res += '\0';
    file.close();
  }
  else
    std::cout << "Unable to open file" << std::endl;
  return (res);
}

unsigned char *CryptoClass::ConvertToUnsigned(std::string str)
{
  unsigned char *res;

  res = new unsigned char[str.length() + 1];
  strcpy((char *) res, str.c_str());
  res[str.length()] = '\0';
  return (res);
}

CryptoClass::CryptoClass()
{
  publicKey = ConvertToUnsigned(getFileKey("./Server/public.pem"));
  privateKey = ConvertToUnsigned(getFileKey("./Server/private.pem"));
  padding = RSA_PKCS1_PADDING;
}

void printLastError(std::string msg)
{
    char * err = (char *)malloc(130);
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), err);
    std::cout << msg << " ERROR : " << err << std::endl;
    free(err);
}

RSA *CryptoClass::CreateRSA(const unsigned char *key, int pub)
{
  RSA *rsa = NULL;
  BIO *keybio;
  keybio = BIO_new_mem_buf((void*)key, (int)strlen((char*)key));
  if (keybio == NULL)
  {
      printf( "Failed to create key BIO");
      return (NULL);
  }
  if (pub == 1)
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, NULL , 0, NULL);
  else
    rsa = PEM_read_bio_RSAPrivateKey(keybio, NULL , 0, NULL);
  if(rsa == NULL)
    std::cout << "Failed to create RSA" << std::endl;
  return (rsa);
}

unsigned char *CryptoClass::PublicEncrypt(std::string data)
{
  //int len_result;
  RSA *rsa = CreateRSA(publicKey, 1);
  const unsigned char *tmp_data;
  unsigned char *res;

  if (rsa == NULL)
    return (NULL);
  res = new unsigned char [4098];
  tmp_data = ConvertToUnsigned(data);
  if ((len = RSA_public_encrypt(data.length() + 1, tmp_data, res, rsa, padding)) == -1)
    {
      printLastError("Public Encrypt failed ");
      return (NULL);
    }
  return (res);
}

unsigned char *CryptoClass::PrivateDecrypt(unsigned char *tmp_data)
{
  //  int len_result;
  RSA *rsa = CreateRSA(privateKey, 0);
  unsigned char *res;

  if (rsa == NULL)
    return (NULL);
  res = new unsigned char [1000];
  int maxSize = RSA_size(rsa);
  
  unsigned char *output = (unsigned char *) malloc(maxSize * sizeof(char));
//int bytes = RSA_public_encrypt((int)[sizeof(Data)], [4096], output, publicKey, RSA_PKCS1_PADDING);
  if (RSA_private_decrypt(strlen((char*)tmp_data), output, res, rsa, padding) == -1)
    {
      printLastError("Private Decrypt failed ");
      return (NULL);
    }
  return (res);
}
