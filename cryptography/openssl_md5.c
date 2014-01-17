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


/*
	Compilation:
	gcc openssl_md5.c -lcrypto
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include <openssl/evp.h>

void md5(
	const void *content, ssize_t content_len, 
	char *result, ssize_t result_len)
{
	assert(result_len==33); // md5+1 length

	int i, j;
	EVP_MD_CTX mdctx;
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	EVP_DigestInit(&mdctx, EVP_md5());
	EVP_DigestUpdate(&mdctx, content, (size_t) content_len);
	EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
	EVP_MD_CTX_cleanup(&mdctx);

	char byte[3];
	j=0;
	for(i=0; i<md_len; i++)
	{
		snprintf(byte, sizeof(byte), "%02x", md_value[i]);
		result[j++]=byte[0];
		result[j++]=byte[1];
	}
	
	result[j]=0;
}

int main()
{
	char result[33];
	char *data = "Test string";
	md5(data, strlen(data), result, sizeof(result));

	printf("%s\n", result);

	return 0;
}




