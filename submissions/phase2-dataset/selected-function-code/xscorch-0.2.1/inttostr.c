char *inttostr(char *d, sdword num, udword digits) {
/* Integer-To_String
   Converts integer <num> to a string with <digits> digits. If
   <digits> is less than number of digits in <num>, then the
   _least_ significant digits are copied. If digits is larger,
   then leading zeroes will be copied. If <digits> = 0, then the
   number will simply be copied without leading zeroes. Note for
   negative numbers: the "-" sign is considered to take up a digit
   space, so be warned... The dest buffer should be at least
   <digits> + 1 characters long. */

   char *dstart = d;
   udword mag = 1;      /* What magnitude are we on right now? */

   if(!d) return(NULL);

   /* Handling for negative numbers */
   if (num < 0) {
      *d = '-';
      d++;
      num = -num;
      digits--;
      if(!digits) {
         *d = '\0';
         return(dstart);
      }
   }

   /* Figure out how many digits to bother with */
   if(digits > 0) {

      /* Print specified number of digits */
      while(--digits) mag *= 10;
   } else {

      /* Print all digits */
      while (num / (mag * 10) > 0) mag *= 10;
   }

   /* Start copying off the digits :) */
   while (mag) {
      *d = (num / mag) % 10 + '0';
      d++;
      num %= mag;
      mag /= 10;
   }

   /* Set ending NULL on the destination string (kinda important..) */
   *d = '\0';
   return(dstart);

}
