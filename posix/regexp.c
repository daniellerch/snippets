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

/* For more information visit www.daniellerch.me */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// {{{ match()
int match(const char *string, const char *pattern, 
      char *result[], size_t len)
{
   int r;
   regex_t expr;
   regmatch_t rm[len];
   result[0]=0;
   int pos=0;

   if( (r=regcomp(&expr, pattern, REG_EXTENDED))!= 0 ) 
      return 0;
   
   if( (r=regexec(&expr, string, len, rm, 0))!=0 )
   {
      regfree(&expr);
      return 0;
   }

   for(pos=0; pos<len; pos++)
   {
      if(rm[pos].rm_so<0)
      {
         result[pos] = malloc(1);
         result[pos][0]=0;
         continue;
      }

      if(rm[pos].rm_so>=0)
      {
         result[pos] = malloc(rm[pos].rm_eo-rm[pos].rm_so+1);
         strncpy(result[pos], &string[rm[pos].rm_so], 
               rm[pos].rm_eo-rm[pos].rm_so+1);
         result[pos][rm[pos].rm_eo-rm[pos].rm_so]=0;
      }
   }

   regfree(&expr);
   return 1;
}
// }}}


int main()
{
   const char *text = "http://www.daniellerch.me/snippets.html";
   const char *patt = "www.(.*).me/(.*)$";

   int i;
   int nres=3;
   char *result[nres];
   if(match(text, patt, result, nres))
   {
      for(i=0; i<nres; i++)
         printf("result %d: %s\n", i, result[i]);

      /* 
         print:
         result 0: www.daniellerch.me/snippets.html
         result 1: daniellerch
         result 2: snippets.html
      */

   }

   for(i=0; i<nres; i++)
      free(result[i]);

   return 0;
}


