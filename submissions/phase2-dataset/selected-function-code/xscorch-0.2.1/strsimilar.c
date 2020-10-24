/* strsimilar
   Sloppy string comparison.
   Basically, make sure the strings are the same, allowing for case and
   punctuation.  The return type is bool.  This function is courtesy of
   Jacob Lundberg.  */

   if(A == B) return(true);
   if(A == NULL || B == NULL) return(false);

   while(*A != '\0' && *B != '\0') {
      /* next valid A char */
      while(!( (*A == '\0') || (*A >= '0' && *A <= '9') ||
               (*A >= 'A' && *A <= 'Z') || (*A >= 'a' && *A <= 'z') ))
         ++A;
      /* next valid B char */
      while(!( (*B == '\0') || (*B >= '0' && *B <= '9') ||
               (*B >= 'A' && *B <= 'Z') || (*B >= 'a' && *B <= 'z') ))
         ++B;
      /* test for disparate strings at this char pair */
      if( ((*A >= 'a' && *A <= 'z') ? (*A - ('a' - 'A')) : *A) !=
          ((*B >= 'a' && *B <= 'z') ? (*B - ('a' - 'A')) : *B) )
         break;
      ++A;
      ++B;
   }

   /* If both made it to the end then they're ``equal'' enough for us. */
   return(*A == '\0' && *B == '\0');

}
