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


void replace(char *str, size_t slen, 
                 const char *pattern, const char *replacement)
{ 
   int n_src=0; 
   char *ptr = str; 
   char *ptr2 = str; 
   
   while((ptr=strstr(ptr, pattern))) 
   { 
      n_src++; 
      ptr++; 
   }
   int new_size = strlen(str) + 
      n_src*(strlen(replacement)-strlen(pattern)); 

   char string[new_size+1]; 
   string[0]=0; 
   ptr = str; 

   while((ptr=strstr(ptr, pattern))) 
   { 
      int len = ptr-ptr2; 
      strncat(string, ptr2, len); 
      strncat(string, replacement, sizeof(string)); 
      string[sizeof(string)-1]=0; 
      ptr2 = ptr+strlen(pattern); 
      ptr  = ptr+strlen(pattern); 
   } 

   int len = ptr-ptr2; 
   strncat(string, ptr2, len); 
   
   snprintf(str, slen, "%s", string);
}

int main()
{
	char buffer[32];
	
	strcpy(buffer, "Hello Wolrd!");
	replace(buffer, sizeof(buffer), "Wolrd", "World");
	printf("%s\n", buffer);

	return 0;
}


