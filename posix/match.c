/*
   Copyright (c) 2008 Daniel Lerch Hostalot <dlerch@gmail.com>

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
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// {{{ match()
int match(const char *string, const char *pattern)
{
   int r;
   regex_t expr;
   regmatch_t rm;

   if( (r=regcomp(&expr, pattern, REG_EXTENDED))!= 0 ) 
      return 0;
   
   if( (r=regexec(&expr, string, 0, &rm, 0))!=0 )
   {
      regfree(&expr);
      return 0;
   }

   regfree(&expr);
   return 1;
}
// }}}

int main(int argc, char *argv[])
{
	if(argc!=3)
	{
		printf("Usage: %s <file> <regex>\n", argv[0]);
		return 0;
	}

	FILE *f = fopen(argv[1], "r");
	if(!f)
	{
		perror("fopen()");
		return 0;
	}

	while(!feof(f))
	{
		char buf[256];
		fgets(buf, sizeof(buf), f);
		
		if(match(buf, argv[2]))
			printf("%s", buf);
	}

	fclose(f);
	return 0;
}






