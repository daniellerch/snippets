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




#include <stdio.h>
#include <string.h>

void lpad(char *s, size_t slen, char padd_char, size_t total_len)
{
   if(!s)
      return;

   if(strlen(s)<total_len) 
   {
      int i;
      int tlen = total_len;
      
      if(tlen>slen)
         tlen=slen;
      
      char string[tlen+1]; 

      strncpy(string, s, sizeof(string)); 
      string[sizeof(string)-1]=0;

      for(i=0; i<tlen-strlen(string); i++) 
         s[i]=padd_char;

      s[i]=0;
      strncat(s, string, tlen);
      s[tlen]=0;
   }
}

void rpad(char *s, size_t slen, char padd_char, size_t total_len)
{
   if(strlen(s)<total_len)
   { 
      int i;
      int len = strlen(s);
      int tlen = total_len;
      
      if(tlen>slen)
         tlen=slen;

      for(i=len; i<tlen; i++)
         s[i]=padd_char;
      s[tlen]=0;
   }
}

int main()
{
	char buffer[64];

	strcpy(buffer, "test1");
	rpad(buffer, 10, 'x', sizeof(buffer));
	printf("%s\n", buffer);

	strcpy(buffer, "test1");
	lpad(buffer, 10, 'x', sizeof(buffer));
	printf("%s\n", buffer);


	return 0;
}





