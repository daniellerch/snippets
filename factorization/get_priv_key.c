/*
   Copyright (c) 2006 Daniel Lerch Hostalot <dlerch@gmail.com>

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
	After factorization, it gets the orifinal RSA key:
   ==================================================================

	Given a RSA key in PEM format, we cant extract the modulus and the
	exponent with:

   openssl -in pubkey.pem -pubin -text -modulus  

	If we can factorize the modulus, we can obtain the original RSA key.

   Compilation:
   gcc get_priv_key.c -lssl -o get_priv_key

*/


#include <stdio.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>

int main (int argc, char **argv) 
{
   RSA *keypair = RSA_new();

   BN_CTX *ctx = BN_CTX_new();
   BN_CTX_start(ctx);

   BIGNUM *n = BN_new();
   BIGNUM *d = BN_new();
   BIGNUM *e = BN_new();
   BIGNUM *p = BN_new();
   BIGNUM *q = BN_new();
   BIGNUM *dmp1 = BN_new();
   BIGNUM *dmq1 = BN_new();
   BIGNUM *iqmp = BN_new();

   BIGNUM *r0 = BN_CTX_get(ctx);
   BIGNUM *r1 = BN_CTX_get(ctx);
   BIGNUM *r2 = BN_CTX_get(ctx);
   BIGNUM *r3 = BN_CTX_get(ctx);


   if (argc!=4) 
   {
      printf ("Usage: %s [p] [q] [exp] \n", argv[0]);
      return 0;
   }

   BN_dec2bn(&p, argv[1]);
   BN_dec2bn(&q, argv[2]);
   BN_dec2bn(&e, argv[3]);

   if(BN_cmp(p, q)<0)
   {
      BIGNUM *tmp = p;
      p = q;
      q = tmp;
   }

   // n
   BN_mul(n, p, q, ctx);

   // d
   BN_sub(r1, p, BN_value_one()); // p-1 
   BN_sub(r2, q, BN_value_one()); // q-1/
   BN_mul(r0, r1, r2, ctx);       // (p-1)(q-1)
   BN_mod_inverse(d, e, r0, ctx); // d 

   // d mod (p-1)
   BN_mod(dmp1, d, r1, ctx); 

   // d mod (q-1)
   BN_mod(dmq1, d, r2, ctx); 

   // inverse of q mod p 
   BN_mod_inverse(iqmp, q, p, ctx);

   // RSA keys
   keypair->n = n;
   keypair->d = d;
   keypair->e = e;
   keypair->p = p;
   keypair->q = q;
   keypair->dmq1 = dmq1;
   keypair->dmp1 = dmp1;
   keypair->iqmp = iqmp;
   
   PEM_write_RSAPrivateKey(stdout, keypair, NULL, NULL, 0, NULL, NULL); 

   BN_CTX_end(ctx);
   BN_CTX_free(ctx);
   RSA_free(keypair);
   return 0;
}





