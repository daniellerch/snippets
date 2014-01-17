/*
   Copyright (c) 2012 Daniel Lerch Hostalot <dlerch@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a 
   copy of this software and associated documentation files (the "Software"), 
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense, 
   and/or sell copies of the Software, and to permit persons to whom the 
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in 
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
   DEALINGS IN THE SOFTWARE.
*/

/* For more information visit www.daniellerch.me */



/*
	Compile:
	$ gcc aes_cbc_encrypt.c -lcrypto

	Decrypt with:
	$ openssl aes-128-cbc -in encrypted.bin \
		-K 000102030405060708090A0B0C0D0E0F \
		-iv 01020102010201020102010201020102 -d

*/




#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/aes.h>


int aes_cbc_encrypt(
	const char *key, 
	const char *iv,
	const char *data, 
	size_t data_len, 
	const char *)
{

	EVP_CIPHER_CTX ctx;
	EVP_EncryptInit(&ctx, EVP_aes_128_cbc(), key, iv);

	char *ret=malloc(
}


int main(int argc, char *argv[])
{
	if(argc!=5)
	{
		printf("Usage: %s <hex key> <hex IV> <input file> <output file>\n", 
			argv[0]);
		return 0;
	}

	unsigned char key[16];
	unsigned char iv[16];

	/* Read Key and IV from main parameters: it is insecure  */
	sscanf(argv[1], "%02X%02X%02X%02X%02X%02X%02X%02X"
						 "%02X%02X%02X%02X%02X%02X%02X%02X", 
			(int*)&key[0],  (int*)&key[1],  (int*)&key[2],  (int*)&key[3],  
			(int*)&key[4],  (int*)&key[5],  (int*)&key[6],  (int*)&key[7],  
			(int*)&key[8],  (int*)&key[9],  (int*)&key[10], (int*)&key[11], 
			(int*)&key[12], (int*)&key[13], (int*)&key[14], (int*)&key[15]);

	sscanf(argv[2], "%02X%02X%02X%02X%02X%02X%02X%02X"
						 "%02X%02X%02X%02X%02X%02X%02X%02X", 
			(int*)&iv[0],  (int*)&iv[1],  (int*)&iv[2],  (int*)&iv[3],  
			(int*)&iv[4],  (int*)&iv[5],  (int*)&iv[6],  (int*)&iv[7],  
			(int*)&iv[8],  (int*)&iv[9],  (int*)&iv[10], (int*)&iv[11], 
			(int*)&iv[12], (int*)&iv[13], (int*)&iv[14], (int*)&iv[15]);


	if(aes_cbc_encrypt(key, iv, argv[2], argv[3])<0)
	{
		puts("Encryption Error");
		return 0;
	}
	
	return 0;
}


