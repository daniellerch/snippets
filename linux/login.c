/*
   Copyright (c) 2007 Daniel Lerch Hostalot <dlerch@gmail.com>

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
	gcc login.c -lcrypt
*/


#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <shadow.h>
#include <unistd.h>

// {{{ auth_check_pass()
int auth_check_pass(const char *clear_pass, const char *encrypted_pass)
{
	char *encrypted = crypt(clear_pass, encrypted_pass);

	if (strcmp(encrypted, encrypted_pass) == 0)
		return 1;
	else
		return 0;
}
// }}}

// {{{ auth_is_valid_user()
int auth_is_valid_user(const char *user, const char* pass)
{
	struct passwd *pw = getpwnam(user);
	if(pw==NULL)
		return 0;

	// password in /etc/shadow
	if(strcmp(pw->pw_passwd, "x")==0)
	{
		/* You must be root */
		struct spwd *sp = getspnam(user);
		if(sp==NULL)
			return 0;

		if(auth_check_pass(pass, sp->sp_pwdp))
			return 1;
	}

	// password in /etc/passwd
	else
	{
		if(auth_check_pass(pass, pw->pw_passwd))
			return 1;
	}

	return 0;
}
// }}}

int main()
{
	if(auth_is_valid_user("user", "pass"))
		puts("Login ok");
	else
		puts("Auth error");
	
	return 0;	
}


