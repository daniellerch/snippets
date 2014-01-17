/*
   Copyright (c) 2012 Daniel Lerch Hostalot <dlerch@gmail.com>

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
   gcc gtk_tcp_client.c `pkg-config --cflags --libs gtk+-2.0`
*/

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

int main()
{
	GError *err = NULL;
	GSocket *sock = NULL;
	GInetAddress *iaddr = NULL;
	GSocketAddress *saddr = NULL;
	GCancellable *cancel = NULL;

	g_type_init();
	iaddr = g_inet_address_new_from_string("192.168.1.6");
	saddr = g_inet_socket_address_new(iaddr, 9999);

	sock = g_socket_new(
			G_SOCKET_FAMILY_IPV4,
			G_SOCKET_TYPE_STREAM,
			G_SOCKET_PROTOCOL_TCP,
			&err);

	if(!g_socket_connect(sock, saddr, cancel, &err))
	{
		printf("Cannot connect\n");
		return -1;
	}


	char buf[256];
	strncpy(buf, "hi server\n", sizeof(buf));
	g_socket_send(sock, buf, strlen(buf), cancel, &err);
	g_socket_receive(sock, buf, sizeof(buf), cancel, &err);

	printf("received from server:\n%s\n", buf);

	return 0;
}
