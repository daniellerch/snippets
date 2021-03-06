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

char* split(char *str, const char *token, char *res, size_t reslen)
{
   if(!str)
      return str;

   if(!reslen)
      return str;

   snprintf(res, reslen, "%s", str);

   char *p = strstr(res, token);
   if(!p)
   {
      return NULL;
   }

   *p = 0;

   return str+strlen(res)+strlen(token);
}


int main()
{
	char buffer[16];
	char *str = "test1:=test2:=test3:=test4";

   while((str = split(str, ":=", buffer, sizeof(buffer))))
      printf("%s\n", buffer);
   printf("%s\n", buffer);

	return 0;
}


