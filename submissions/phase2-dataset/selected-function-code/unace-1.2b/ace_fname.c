CHAR *ace_fname(CHAR * s, thead * head, INT nopath, unsigned int size)
{
   unsigned int i;
   char *cp;

   i = (*(tfhead *) head).FNAME_SIZE;
   if (i > (size - 1))
     i = size - 1;
   strncpy(s, (*(tfhead *) head).FNAME, i);
   s[i] = 0;

   if (nopath)
   {
      cp=strrchr(s, '\\');
      if (cp)
         memmove(s, cp+1, strlen(cp));
   }
#if (DIRSEP!='\\')                  // replace msdos directory seperator
   else
   {                                // by current OS seperator
      cp=s;
      while ((cp=strchr(cp, '\\'))!=NULL)
         *cp++=DIRSEP;
   }
#endif

   cp = s;
   while (*cp == '/') cp++;
   if (cp != s)
     memmove(s, cp, strlen(cp) + 1);

   return s;
}