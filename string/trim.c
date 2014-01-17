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

void trim(char *str)
{
   if(!str)
      return;

   char *ptr1 = str;
   char *ptr2 = str;

   while(isspace(*ptr2)) 
      ptr2++; 

   while(*ptr2!=0) 
      *ptr1++ = *ptr2++;
         
   ptr1--;
   while(isspace(*ptr1)) 
      ptr1--; 

   *(ptr1+1) = 0;
}

int main()
{
	char buffer[64];

	strcpy(buffer, "  test1\n");
	trim(buffer);
	printf("|%s|\n", buffer);
	
	strcpy(buffer, "test2\n");
	trim(buffer);
	printf("|%s|\n", buffer);

	strcpy(buffer, "\t\ttest 3\t\n");
	trim(buffer);
	printf("|%s|\n", buffer);

	strcpy(buffer, "\t\ttest\n4\t\n");
	trim(buffer);
	printf("|%s|\n", buffer);

	return 0;
}



