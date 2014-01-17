/*
   Copyright (c) 2013 Daniel Lerch Hostalot <dlerch@gmail.com>

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

#include <stdio.h>
#include <string.h>

int randstr(char *s, size_t slen, int rndlen, const char *validchars)
{
	if(rndlen > slen-1)
		return -1;

	int i;
	int l=strlen(validchars);
	int seed = 0;

   // Use /dev/random for high quality randomness (man urandom).
   FILE *random_file = fopen("/dev/urandom","r");
   if(!random_file)
		return -2;
      
	seed = getc(random_file);
   srandom(seed);

   for(i=0; i<rndlen; i++)
   {
      int idx = random()%l;
      s[i]=validchars[idx];
   }

   s[i]=0;
   fclose(random_file);
	return 0;
}

int main()
{
	char buffer[16];

	if(randstr(buffer, sizeof(buffer), 10, "0123456789ABCDEF")!=0)
	{
		// Error processing ...
	}
	printf("%s\n", buffer);

	if(randstr(buffer, sizeof(buffer), 10, 
	"0123456789ABCDEFabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")!=0)
	{
		// Error processing ...
	}
	printf("%s\n", buffer);

	return 0;
}





