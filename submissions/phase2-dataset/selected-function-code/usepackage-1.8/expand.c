{
   static char newpath[256];
   static char username[32];
   struct passwd* pwent;
   char* home;
   int i,j;

   i = 0;
   newpath[0] = '\0';

   if (filepath[i] == '~')
   {
      i++;

      if (!filepath[i] || filepath[i] == '/')
      {
      	 home = getenv("HOME");
      	 if (home)
      	 {
     	    strcat(newpath, home);
      	 }
      	 else
      	 {
	    pwent = getpwuid(getuid());

	    if (!pwent)
	    {
	       fprintf(stderr, "usepackage: cannot obtain home directory.\n");
	       return(filepath);
	    }
	    
	    strcat(newpath, pwent->pw_dir);
	 }
      }
      else
      {
         j = i;
         while (filepath[i] && (filepath[i] != '/')) i++;
         strncpy(username, filepath + j, i - j);
         username[i-j] = '\0';
	 pwent = getpwnam(username);

	 if (!pwent)
	 {
	    fprintf(stderr,
                    "usepackage: cannot obtain home directory of user `%s'.\n",
                    username);
	    return(filepath);
	 }

         strcat(newpath, pwent->pw_dir);
      }
   }

   strcat(newpath, filepath + i);

   return(newpath);
}
