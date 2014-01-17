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

/* For more information visit www.daniellerch.me */

#include <gmpxx.h>
#include <iostream>

int main(int argc, char *argv[])
{
   using namespace std;

   mpz_class n, x, u, v, a, b, r;

   if(argc!=2)
   {
      cerr << "Usage: " << argv[0] << " <n number>" << endl;
      return 0;
   }
   
   n = argv[1];
   mpz_sqrt(x.get_mpz_t(), n.get_mpz_t()); 
   u = 2*x + 1;
   v = 1;
   r = x*x - n;

   while(r!=0)
   {
      if(r>0)
      {
         while(r>0)
         {
            r -= v;
            v += 2;
         }
      }
      else 
      {
         r += u;
         u += 2;
      }
   }

   a = (u+v-2)/2;
   b = (u-v)/2;
   
   cout << "a: " << a << endl;
   cout << "b: " << b << endl;

   return 0;
}







