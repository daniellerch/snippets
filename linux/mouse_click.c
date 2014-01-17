/*
   Copyright (c) 2010 Daniel Lerch Hostalot <dlerch@gmail.com>

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
	Compilation:
	gcc mouse_click.c -lX11 -lXtst
*/

#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/types.h>
#include<fcntl.h>
#include<linux/input.h>
#include<fcntl.h>
#include<linux/input.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/extensions/Xrandr.h>
#include<X11/extensions/XTest.h>

void mouse_move(Display *display, int x, int y)
{
	XTestFakeMotionEvent(display, -1, x, y, CurrentTime);
	XFlush(display);
}

void mouse_button_press(Display *display, int button)
{
	XTestFakeButtonEvent(display, button, True, CurrentTime);
	XFlush(display);
}

void mouse_button_release(Display *display, int button)
{
	XTestFakeButtonEvent(display, button, False, CurrentTime);
	XFlush(display);
}


int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		printf("Usage: %s [x coord] [y coord]\n\n", argv[0]);
		return 0;
	}

	int x = atoi(argv[1]);
	int y = atoi(argv[2]);

	Display *display = XOpenDisplay(getenv("DISPLAY"));
	mouse_move(display, x, y);

	mouse_button_press(display, 1);
	mouse_button_release(display, 1);

	return 0;
}

