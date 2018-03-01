#include "OpenSLL.h"

int					OpenSSL::PublicEncrypt(unsigned char *data, int length, unsigned char *key, unsigned char *encrypted)
{
	std::cout << data << std::endl;
	RSA * rsa = createRSA(key);
	int result = RSA_public_encrypt(length, data, encrypted, rsa, padding);
	return result;
}

int					OpenSSL::PublicDecrypt(unsigned char *data, int length, unsigned char *key, unsigned char *decrypted)
{
	RSA * rsa = createRSA(key);
	int  result = RSA_public_decrypt(length, data, decrypted, rsa, padding);
	return result;
}

RSA*				OpenSSL::createRSA(unsigned char *key)
{
	RSA*			rsa = NULL;
	BIO*			keybio = NULL;

	keybio = BIO_new_mem_buf(key, -1);
	if (keybio == NULL)
	{
		printf("Failed to create key BIO");
		return (0);
	}
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	
	if (rsa == NULL)
	{
		printf("Failed to create RSA");
		return (0);
	}
	return rsa;
}

OpenSSL::OpenSSL()
{
	this->padding = RSA_PKCS1_PADDING;
}


OpenSSL::~OpenSSL()
{
}
