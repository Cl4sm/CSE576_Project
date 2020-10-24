INT  proc_vol(void)                     // opens volume
{
   INT  i;
   CHAR s[PATH_MAX + 80];

   // if f_allvol_pr is 2 we have -y and should never ask
   if ((!fileexists_insense(aname) && f_allvol_pr != 2) || !f_allvol_pr)
   {
      do
      {
         sprintf(s, "Ready to process %s?", aname);
         beep();
         i = wrask(s);                  // ask whether ready or not
         f_allvol_pr = 0;
         if(i == 1)                     // "Always" --> process all volumes
             f_allvol_pr = 1;
         if (i >= 2)
         {
            f_err = ERR_FOUND;
            return 0;
         }
      }
      while (!fileexists_insense(aname));
   }

   if (!open_archive(1))                // open volume
   {                            
      printf("\nError while opening archive. File not found or archive broken.\n");
      f_err = ERR_OPEN;
      return 0;
   }

   return 1;
}