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


/*
	Compilation:
	$ gcc sshbatch.c -lutil 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pty.h>
#include <utmp.h>


#define SSH_PATH "/usr/bin/ssh"
#define SCP_PATH "/usr/bin/scp"
#define FINGERPRINT_MSG "(yes/no)? "
#define PASSWORD_MSG "password: "
#define MITM_MSG "man-in-the-middle attack"

int is_msg(const char *buffer, const char *msg)
{
   char *ptr = strstr(buffer, msg);
   if(!ptr)
      return 0;
   
   return 1;
}

void usage(const char *pn)
{
   printf("Usage: %s passw '<pass>' host <user@host> "
          "exec '<command> [acceptfp]'\n", pn);
   printf("       %s passw '<pass>' host <user@host> "
          "send <local path> <remote path> [acceptfp]\n", pn);
   printf("       %s passw '<pass>' host <user@host> "
          "get <remote path> <local path> [acceptfp]\n", pn);
   exit(0);
}

int main(int argc, char **argv)
{
   int i;
   enum {ssh, scp} type;
   char *bin_path = NULL;
   int pty, child;
   int ret;
   char command[256];
   char password[64];
   int acceptfp = 0;

   /* Parse arguments */
   if(argc < 6)
      usage(argv[0]);
   
   if(strcmp(argv[1], "passw")!=0)
      usage(argv[0]);

   snprintf(password, sizeof(password), "%s", argv[2]);
   int len=strlen(argv[2]);
   for(i=0; i<len; i++)
      argv[2][i]='X';


   if(strcmp(argv[3], "host")!=0)
      usage(argv[0]);

   if(strcmp(argv[5], "exec")==0)
   {
      bin_path = SSH_PATH;
      type = ssh;
      snprintf(command, sizeof(command), 
         "%s %s '%s'", bin_path, argv[4], argv[6]);
   
      if(argc==8 && strcmp(argv[7], "acceptfp")==0)
         acceptfp = 1;
   }

   else if(strcmp(argv[5], "send")==0)
   {
      if(argc < 8)
         usage(argv[0]);

      bin_path = SCP_PATH;
      snprintf(command, sizeof(command), 
         "%s -r %s %s:%s", bin_path, argv[6], argv[4], argv[7]);

      if(argc==9 && strcmp(argv[8], "acceptfp")==0)
         acceptfp = 1;
   }
   else if(strcmp(argv[5], "get")==0)
   {
      if(argc < 8)
         usage(argv[0]);

      bin_path = SCP_PATH;
      snprintf(command, sizeof(command), 
         "%s -r %s:%s %s", bin_path, argv[4], argv[6], argv[7]);

      if(argc==9 && strcmp(argv[8], "acceptfp")==0)
         acceptfp = 1;
   }
   else
      usage(argv[0]);


   /* Pseudo terminal */
   child = forkpty(&pty,0,0,0);
   if(child == -1)
   {
      perror("forkpty()");
      exit(-1); 
   }

   /* Command execution */
   if(!child)
   {
      system(command);
      exit(-1);
   }

   /* Get last output line */
   char buffer[128];

   while(!is_msg(buffer, FINGERPRINT_MSG) && 
         !is_msg(buffer, MITM_MSG) &&
         !is_msg(buffer, PASSWORD_MSG))
   { 
      ret = read(pty, buffer, sizeof(buffer));
      if(ret>0 && ret < sizeof(buffer))
         buffer[ret]=0;
   }


   /* Check mitm */
   if(is_msg(buffer, MITM_MSG))
   {
      puts("error: man-in-the-middle attack!? check fingerprint.");
      return 0;
   }

   /* Fingerprint */
   if(is_msg(buffer, FINGERPRINT_MSG))
   {
      if(acceptfp)
      {
         puts("accept fingerprint.");
         write(pty, "yes\n", 4);   

         while(!is_msg(buffer, PASSWORD_MSG))
         { 
            ret = read(pty, buffer, sizeof(buffer));
            if(ret>0 && ret < sizeof(buffer))
               buffer[ret]=0;
         }
      }
      else
      {
         puts("error: fingerprint not accepted, use 'acceptfp'.");
         write(pty, "no\n", 3);   

         ret = read(pty, buffer, sizeof(buffer));   
         if(ret>0 && ret < sizeof(buffer))
         buffer[ret]=0;

         return 0;
      }
   }

   /* Password prompt */
   if(!is_msg(buffer, PASSWORD_MSG))
   {
      puts("error: unknown ssh response:");
      printf("'%s'\n", buffer);
      return 0;
   }


   /* Send password */
   write(pty, password, sizeof(password));
   write(pty, "\n", 1);
   char ch;
   read(pty, &ch, 3);

   /* Connect pty to standard output */
   pid_t rpid;
   if((rpid=fork())==0)
   {
      while(read(pty, &ch, 1) != -1)
		{
			if(ch!=13)
      	   write(1, &ch, 1);
		}
      exit(0);
   }
   
   /* wait child */
   waitpid(rpid, &ret, 0);
   waitpid(child, &ret, 0);

   return 0;
}


