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
	Compilation:
	gcc openssl_aes_cbc.c -lcrypto
*/


#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

// {{{ aes_encrypt()
unsigned char* aes_encrypt(
	unsigned char *passw, unsigned char* plaintext, int *len)
{
   EVP_CIPHER_CTX ctx;
   unsigned char key[32], iv[32];

   EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), NULL, 
      passw, strlen((char*)passw), 1, key, iv);

   EVP_CIPHER_CTX_init(&ctx);
   EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);

   int c_len = *len + AES_BLOCK_SIZE;
   int f_len = 0;
   unsigned char *ciphertext = malloc(c_len);

   EVP_EncryptInit_ex(&ctx, NULL, NULL, NULL, NULL);
   EVP_EncryptUpdate(&ctx, ciphertext, &c_len, plaintext, *len);
   EVP_EncryptFinal_ex(&ctx, ciphertext+c_len, &f_len);

   *len = c_len + f_len;
   EVP_CIPHER_CTX_cleanup(&ctx);
   
   return ciphertext;
}
// }}}

// {{{ aes_decrypt()
unsigned char* aes_decrypt(
	unsigned char *passw, unsigned char* ciphertext, int *len)
{
   EVP_CIPHER_CTX ctx;
   unsigned char key[32], iv[32];

   EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), NULL, 
      passw, strlen((char*)passw), 1, key, iv);

   EVP_CIPHER_CTX_init(&ctx);
   EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);

   int p_len = *len;
   int f_len = 0;
   unsigned char *plaintext = malloc(p_len + AES_BLOCK_SIZE);

   EVP_DecryptInit_ex(&ctx, NULL, NULL, NULL, NULL);
   EVP_DecryptUpdate(&ctx, plaintext, &p_len, ciphertext, *len);
   EVP_DecryptFinal_ex(&ctx, plaintext+p_len, &f_len);

   *len = p_len + f_len;
  
   EVP_CIPHER_CTX_cleanup(&ctx);

   return plaintext;
}
// }}}

int main(int argc, char **argv)
{
   unsigned char *plain = (unsigned char*)"Hello World!";
   int len=strlen((char*)plain)+1;
	unsigned char *password = (unsigned char*)"secret";

   unsigned char *ciphertext = aes_encrypt(password, plain, &len);
   unsigned char *plaintext = aes_decrypt(password, ciphertext, &len);

   printf("text: %s\n", plaintext);

   free(ciphertext);
   free(plaintext);

   return 0;
}

