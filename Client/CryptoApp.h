#pragma once

#ifndef CryptoApp_H_
# define CryptoApp_H_

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string>
#include <iostream>

class CryptoApp
{
public:
	virtual int			PublicEncrypt(unsigned char *, int, unsigned char *, unsigned char *) = 0;
	virtual int			PublicDecrypt(unsigned char *, int, unsigned char *, unsigned char *) = 0;

public:
	~CryptoApp();
};

#endif // !CryptoApp_H_
