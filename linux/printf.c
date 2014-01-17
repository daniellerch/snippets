/*
   Copyright (c) 2009 Daniel Lerch Hostalot <dlerch@gmail.com>

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

#define COLOR_RED "\e[31m"
#define COLOR_B_RED "\e[31;1m"
#define COLOR_GREEN "\e[32m"
#define COLOR_B_GREEN "\e[32;1m"
#define COLOR_YELLOW "\e[33m"
#define COLOR_B_YELLOW "\e[33;1m"
#define COLOR_BLUE "\e[34m"
#define COLOR_B_BLUE "\e[34;1m"
#define COLOR_MAGENTA "\e[35m"
#define COLOR_BRIGHT "\e[1m"
#define COLOR_CYAN "\e[36m"
#define COLOR_NONE "\e[m"

int main(int argc, char *argv[])
{
    printf(COLOR_RED "hello world\n" COLOR_NONE);
    printf(COLOR_B_RED "hello world\n" COLOR_NONE);
    printf(COLOR_GREEN "hello world\n" COLOR_NONE);
    printf(COLOR_B_GREEN "hello world\n" COLOR_NONE);
    printf(COLOR_YELLOW "hello world\n" COLOR_NONE);
    printf(COLOR_B_YELLOW "hello world\n" COLOR_NONE);
    printf(COLOR_BLUE "hello world\n" COLOR_NONE);
    printf(COLOR_B_BLUE "hello world\n" COLOR_NONE);
    printf(COLOR_CYAN "hello world\n" COLOR_NONE);
    printf(COLOR_MAGENTA "hello world\n" COLOR_NONE);
    printf(COLOR_BRIGHT "hello world\n" COLOR_NONE);
    return 0;
}


