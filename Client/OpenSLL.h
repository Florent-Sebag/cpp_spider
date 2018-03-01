#pragma once

#ifndef OpenSSL_H_
# define OpenSSL_H_

#include "CryptoApp.h"

class OpenSSL : public CryptoApp
{
private:
	int					padding;

public:
	int					PublicEncrypt(unsigned char *, int, unsigned char *, unsigned char *);
	int					PublicDecrypt(unsigned char *, int, unsigned char *, unsigned char *);
	RSA*				createRSA(unsigned char *);

public:
	OpenSSL();
	~OpenSSL();
};

#endif // !OpenSSL_H_
