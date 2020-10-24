/* Integer-To_Hexadecimal
   Converts integer <num> to a string with <digits> digits. If
   <digits> is less than number of digits in <num>, then the
   _least_ significant digits are copied. If digits is larger,
   then leading zeroes will be copied. If <digits> = 0, then the
   number will simply be copied without leading zeroes. Note for
   negative numbers: the "-" sign is considered to take up a digit
   space, so be warned... The dest buffer should be at least
   <digits> + 1 characters long. */

   char *dstart = d;
   unsigned int mag = 0;      /* What magnitude are we on right now? */

   if(!d) return(NULL);

   /* Figure out how many digits to bother with */
   if(digits > 0) {

      /* Print specified number of digits */
      while(--digits) mag += 4;
   } else {

      /* Print all digits */
      while ((num >> mag) > 0) mag += 4;
   }

   /* Start copying off the digits :) */
   while (mag) {
      *d = (num >> mag) & 0xf;
      #ifdef PREFER_UPPER
      if(*d > 9) *d += 'A';
      #else
      if(*d > 9) *d += 'a';
      #endif
      else *d += '0';
      d++;
      mag -= 4;
   }

   /* Set ending NULL on the destination string (kinda important..) */
   *d = '\0';
   return(dstart);

}
