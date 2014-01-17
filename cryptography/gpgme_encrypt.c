/*
   Copyright (c) 2007 Daniel Lerch Hostalot <dlerch@gmail.com>

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
	gcc gpgme_encrypt.c `gpgme-config --cflags --libs` -D_FILE_OFFSET_BITS=64 
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <gpgme.h>

#define BUF_SIZE 512

int gpg_encrypt(char *fingerprint, char *src_path, char *dst_path)
{
	gpgme_ctx_t ctx;
	gpgme_error_t err;
	gpgme_data_t in, out;
	gpgme_key_t key[2] = { NULL, NULL};
	gpgme_encrypt_result_t result;
	char buf[BUF_SIZE + 1];
	int ret;
	FILE *f;

	gpgme_check_version(NULL);
	setlocale(LC_ALL, "");
	gpgme_set_locale(NULL, LC_CTYPE, setlocale (LC_CTYPE, NULL));
	gpgme_set_locale(NULL, LC_MESSAGES, setlocale (LC_MESSAGES, NULL));

	if( (err=gpgme_engine_check_version (GPGME_PROTOCOL_OpenPGP)) != 0)
	{
		fprintf(stderr, "%s: %s\n", gpgme_strsource(err), gpgme_strerror (err));
		return EXIT_FAILURE;
	}

	if( (err=gpgme_new(&ctx)) != 0)
	{
		fprintf(stderr, "%s: %s\n", gpgme_strsource(err), gpgme_strerror (err));
		return EXIT_FAILURE;
	}

	gpgme_set_armor(ctx, 1);

	if( (err=gpgme_data_new_from_file (&in, src_path, 1)) != 0)
	{
		fprintf(stderr, "%s: %s\n", gpgme_strsource(err), gpgme_strerror (err));
		return EXIT_FAILURE;
	}

	if( (err=gpgme_data_new(&out)) != 0)
	{
		fprintf(stderr, "%s: %s\n", gpgme_strsource(err), gpgme_strerror (err));
		return EXIT_FAILURE;
	}

	if( (err=gpgme_get_key(ctx, fingerprint, &key[0], 0)) != 0)
	{
		fprintf(stderr, "%s: %s\n", gpgme_strsource(err), gpgme_strerror (err));
		return EXIT_FAILURE;
	}

	if( (err=gpgme_op_encrypt(ctx, key, GPGME_ENCRYPT_ALWAYS_TRUST,
					in,out)) !=0)
	{
		fprintf(stderr, "%s: %s\n", gpgme_strsource(err), gpgme_strerror (err));
		return EXIT_FAILURE;
	}

	result = gpgme_op_encrypt_result(ctx);
	if (result->invalid_recipients)
	{
		fprintf(stderr,"Invalid recipient: %s\n",result->invalid_recipients->fpr);
		return EXIT_FAILURE;
	}

	if( (f=fopen(dst_path, "w+")) == NULL)
	{
		perror("fopen()");
		return EXIT_FAILURE;
	}

	if( (ret=gpgme_data_seek (out, 0, SEEK_SET)) != 0)
	{
		err = gpgme_err_code_from_errno (errno);
		fprintf(stderr, "%s: %s\n", gpgme_strsource(err), gpgme_strerror (err));
		return EXIT_FAILURE;
	}

	while( (ret = gpgme_data_read (out, buf, BUF_SIZE)) > 0)
		fwrite (buf, ret, 1, f);

	if(ret<0)
	{
		err = gpgme_err_code_from_errno (errno);
		fprintf(stderr, "%s: %s\n", gpgme_strsource(err), gpgme_strerror (err));
		return EXIT_FAILURE;
	}

	gpgme_key_unref (key[0]);
	gpgme_key_unref (key[1]);
	gpgme_data_release (in);
	gpgme_data_release (out);
	gpgme_release (ctx);

	return EXIT_SUCCESS;
}


int main()
{
	/*
		$ gpg --gen-key
		  ...
	
		$ gpg --list-keys
		  ...
		  pub   2048R/E27C9CC7 ...
	*/
	gpg_encrypt("E27C9CC7", "file", "file.gpg");

	return 0;
}

