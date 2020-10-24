double strtofloat(const char *s, int *succ) {
/* String-To-Float
   Converts string <s> to a double value, if possible. If succ
   is not null, then the return value in <*succ> will be zero
   on success, and on failure it will be a pointer to the
   character which failed PLUS ONE. If <succ> is NULL, then the
   returned value will be zero but there will be no indication
   of the error having occured. */

   /* random thoughts, random code, random bugs, random crashes */
   int state = 0;
   double result = 0;
   long mantissa = 0;
   int posbase = -1;    /* +1 for plus, 0 for minus, -1 for default (plus) */
   int posmant = -1;    /* +1 for plus, 0 for minux, -1 for default (plus) */
   double decval = 0.1; /* power to multiply for decimal digit */

   if(!s) {
      if(succ) *succ = 1;
      return(0);
   }
   if(succ) *succ = 0;

   while(state >= 0) {
      switch(state) {

         /* Initial-state processing
            -> state 1        if character is '+', '-'
            -> state 2        if character is '0'..'9'
            -> state 3        if character is '.'
            -> state -1       on error */
         case 0:
            state = 1;
            if(*s == '+') posbase = 1;
            else if(*s == '-') posbase = 0;
            if(posbase >= 0) break;
            state = 2;
            if(*s >= '0' && *s <= '9') result = (*s - '0');
            else if(*s == '.') state = 3;
            else state = -1;
            break;

         /* Require-whole-number-or-decimal state
            -> state 2        if character is '0'..'9'
            -> state 3        if character is '.'
            -> state -1       if error */
         case 1:
            state = 2;
            if(*s >= '0' && *s <= '9') result = (result * 10) + (*s - '0');
            else if(*s == '.') state = 3;
            else state = -1;
            break;

         /* Optional-whole-number-or-decimal state
            -> state 2        if character is '0'..'9'
            -> state 4        if character is '.'
            -> state 5        if character is 'e'
            -> state -1       if error
            -> state -2       if finished */
         case 2:
            if(*s >= '0' && *s <= '9') result = (result * 10) + (*s - '0');
            else if(*s == '.') state = 4;
            else if(*s == 'e' || *s == 'E') state = 5;
            else if(!*s) state = -2;
            else state = -1;
            break;

         /* Require-decimal-digit state
            -> state 4        if character is '0'..'9'
            -> state -1       if error */
         case 3:
            state = 4;
            if(*s >= '0' && *s <= '9') {
               result = result + decval * (*s - '0');
               decval /= 10;
            } else state = -1;
            break;

         /* Optional-decimal-digit state
            -> state 4        if character is '0'..'9'
            -> state 5        if character is 'e'
            -> state -1       if error
            -> state -2       if finished */
         case 4:
            if(*s >= '0' && *s <= '9') {
               result = result + decval * (*s - '0');
               decval /= 10;
            } else if(*s == 'e' || *s == 'E') state = 5;
            else if(!*s) state = -2;
            else state = -1;
            break;

         /* Exponential-init state
            -> state 6        if character is '+', '-'
            -> state 7        if character is '0'..'9'
            -> state -1       on error */
         case 5:
            state = 6;
            if(*s == '+') posmant = 1;
            else if(*s == '-') posmant = 0;
            if(posmant >= 0) break;
            state = 7;
            if(*s >= '0' && *s <= '9') mantissa = (*s - '0');
            else state = -1;
            break;

         /* Require-exponential-digit state
            -> state 7        if character is '0'..'9'
            -> state -1       on error */
         case 6:
            state = 7;
            if(*s >= '0' && *s <= '9') mantissa = (mantissa * 10) + (*s - '0');
            else state = -1;
            break;

         /* Optional-exponential-digit state
            -> state 7        if character is '0'..'9'
            -> state -1       if error
            -> state -2       if finished */
         case 7:
            if(*s >= '0' && *s <= '9') mantissa = (mantissa * 10) + (*s - '0');
            else if(!*s) state = -2;
            else state = -1;
            break;

      }
      s++;
      if(succ) (*succ)++;
   }

   if(state == -1) return(0);
   if(!posbase) result = -result;
   if(!posmant) mantissa = -mantissa;
   result = pow(result, mantissa);
   if(succ) *succ = 0;
   return(result);

}
