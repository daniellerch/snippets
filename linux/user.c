/*
   Copyright (c) 2011 Daniel Lerch Hostalot <dlerch@gmail.com>

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
	gcc user.c -lcrypt
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <shadow.h>
#include <sys/time.h>
#include <errno.h>


#define USER_GROUP_ID   1001
#define USER_DEFAULT_ID   1001

/// Seconds in a day
#define DAY (24L*3600L)
/// Seconds in a week
#define WEEK (7*DAY)
/// Time scale used
#define SCALE DAY
/// External function for encrypting the user's password
extern char *crypt(const char *key, const char *salt);


// {{{ user_del_line()
/// Delete the user's line in the specified file (passwd, shadow)
int user_del_line(const char *username, const char* filename)
{
	char *buffer;
	FILE *passwd;
	size_t len;
	int begin;
	int end;
	char needle[256];
	char *start;
	char *stop;
	struct stat statbuf;

	passwd = fopen(filename, "r");
	if (!passwd) 
		return -1;
	
	stat(filename, &statbuf);
	len = statbuf.st_size;
	buffer = (char *) malloc(len * sizeof(char));

	if (!buffer) 
	{
		fclose(passwd);
		return -1;
	}
	
   fread(buffer, len, sizeof(char), passwd);

	fclose(passwd);

	snprintf(needle, 256, "\n%s:", username);
	needle[255] = 0;
	start = strstr(buffer, needle);

	if (!start) 
	{
		begin = 0;
		end = 0;
	}
	else
	{
		start++;
		stop = strchr(start, '\n');
   		begin = start - buffer;
   		end = stop - buffer;
	}

	passwd = fopen(filename, "w");
	if (!passwd) 
		return -1;
	
	fwrite(buffer, (begin - 1), sizeof(char), passwd);
	fwrite(&buffer[end], (len - end), sizeof(char), passwd);

	fclose(passwd);
	return 0;
}
// }}}

// {{{ user_add()
/// Create a valid user account
void user_add(char *username, volatile char *passwd)
{
   struct passwd p;
   struct passwd *pw;
   struct spwd sp;
   FILE *f; 
   int min = 1000;
   int max = 65000;
   char home[256];

   snprintf(home, sizeof(home), "/home/%s", username);

   p.pw_name = (char *)username;
   p.pw_passwd = "x";
   p.pw_uid = USER_DEFAULT_ID;
   p.pw_gid = USER_GROUP_ID;
   p.pw_gecos = "Default User";
   p.pw_dir = home;
   p.pw_shell = "/bin/sh";


   f = fopen("/etc/passwd", "r");

   /* check user and get valid id */
   while ((pw = fgetpwent(f))) 
   {
      if (strcmp(pw->pw_name, p.pw_name) == 0) 
      {
         perror("user_add(): user exists");
         return;
      }

      if ((pw->pw_uid >= p.pw_uid) && (pw->pw_uid < max)
            && (pw->pw_uid >= min)) 
      {
         p.pw_uid = pw->pw_uid + 1;
      }
   }

   fclose(f);

   f = fopen("/etc/passwd", "a+");
   if(!f)
   {
      perror("user_add(): cannot open /etc/passwd");
      return;
   }


   /* add to passwd */
   if (putpwent(&p, f) == -1) 
   {
      perror("user_add(): putpwent() error");
      return;
   }

   fclose(f);


   /* salt */
   struct timeval tv;
   static char salt[40];

   salt[0] = '\0';

   gettimeofday (&tv, (struct timezone *) 0);
   strcat(salt, l64a (tv.tv_usec));
   strcat(salt, l64a (tv.tv_sec + getpid () + clock ()));

   if (strlen (salt) > 3 + 8)  
      salt[11] = '\0';


   /* shadow */
   sp.sp_namp = p.pw_name;
   sp.sp_pwdp = (char*)crypt((const char*)passwd, salt);
   sp.sp_min = 0;
   sp.sp_max = (10000L * DAY) / SCALE;
   sp.sp_lstchg = time((time_t *) 0) / SCALE;
   sp.sp_warn = -1;
   sp.sp_expire = -1;
   sp.sp_inact = -1;
   sp.sp_flag = -1;


   /* add to shadow */
   f = fopen("/etc/shadow", "a+");
   if(!f)
   {
      perror("user_add(): cannot open /etc/shadow");
      return;
   }

   if (putspent(&sp, f) == -1) 
   {
      perror("user_add(): putspent() error");
      return;
   }

   fclose(f);

   /* Create home */
   mkdir(home, 0700);  
   chown(home, p.pw_uid, USER_GROUP_ID);
}
// }}}

// {{{ user_del()
/** Delete specified user
 @param o User structure
 @param username User name to be deleted
 */
void user_del(char *username)
{
   if(user_del_line(username, "/etc/passwd")!=0)
   {
      perror("user_del() can not remove user from /etc/passwd");
      return;
   }


   if(user_del_line(username, "/etc/shadow")!=0)
   {
      perror("user_del() can not remove user from /etc/passwd");
      return;
   }

}
// }}}

// {{{ user_set_password()
/// Set the password for the specified username
void user_set_password(char *username, volatile char* passwd)
{
   FILE *f;
   struct spwd *sp = NULL;

   sp = getspnam(username);
   if(!sp)
   {
      perror("user_set_password() unknown user");
      return;
   }


   /* salt */
   struct timeval tv;
   static char salt[40];

   salt[0] = '\0';

   gettimeofday (&tv, (struct timezone *) 0);
   strcat(salt, l64a (tv.tv_usec));
   strcat(salt, l64a (tv.tv_sec + getpid () + clock ()));

   if (strlen (salt) > 3 + 8)  
      salt[11] = '\0';


   /* shadow */
   sp->sp_pwdp = (char*)crypt((const char*)passwd, salt);


   if(user_del_line(username, "/etc/shadow")!=0)
   {
      perror("user_set_password() cannot modify /etc/shadow");
      return;
   }

   f = fopen("/etc/shadow", "a+");
   if(!f)
   {
      perror("user_set_password(): cannot open /etc/shadow");
      return;
   }

   if (putspent(sp, f) == -1) 
   {
      perror("user_add(): putspent() error");
      return;
   }

   fclose(f);



}
// }}}


int main()
{
	user_add("testuser", "testpass");
	user_set_password("testuser", "testpass2");
	user_del("testuser");
	return 0;
}


