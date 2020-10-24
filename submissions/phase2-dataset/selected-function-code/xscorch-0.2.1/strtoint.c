/* String-To-Integer
   Converts string <s> to a long integer, if possible. If succ
   is not null, then the return value in <*succ> will be zero
   on success, and on failure it will be a pointer to the
   character which failed PLUS ONE. If <succ> is NULL, then the
   returned value will be zero but there will be no indication
   of the error having occured. */

   /* random thoughts, random code, random bugs, random crashes */
   sdword result = 0;
   int state = 0;
   int possign = -1;    /* +1 for plus, 0 for minus, -1 for default (plus) */

   if(!s) {
      if(succ) *succ = 1;
      return(0);
   }
   if(succ) *succ = 0;

   while(state >= 0) {
      switch(state) {

         /* Initial-state processing
            Accepts '+', '-', '0'..'9', 'x', 'o', 'b'
            -> state 1     if character is '+', '-'
            -> state 2     if character is '1'..'9'
            -> state 3     if character is '0'
            -> state 4     if character is 'x'
            -> state 6     if character is 'o'
            -> state 8     if character is 'b'
            -> state -1    if error */
         case 0:
            state = 1;
            if(*s == '+') possign = 1;
            else if(*s == '-') possign = 0;
            if(possign >= 0) break;
            state = 2;
            if(*s == '0') state = 3;
            else if(*s >= '1' && *s <= '9') result = *s - '0';
            else if(*s == 'x' || *s == 'X') state = 4;
            else if(*s == 'o' || *s == 'O') state = 6;
            else if(*s == 'b' || *s == 'B') state = 8;
            else state = -1;
            break;

         /* Require-decimal-number state
            -> state 2     if character is '0'..'9'
            -> state -1    if error */
         case 1:
            state = 2;
            if(*s >= '0' && *s <= '9') result = (result * 10) + (*s - '0');
            else state = -1;
            break;

         /* Optional-decimal-number state
            -> state 2     if character is '0'..'9'
            -> state -1    if error
            -> state -2    if finished */
         case 2:
            if(*s >= '0' && *s <= '9') result = (result * 10) + (*s - '0');
            else if(!*s) state = -2;
            else state = -1;
            break;

         /* Optional-decimal-or-hex-delimiter state
            -> state 2     if character is '0'..'9'
            -> state 4     if character is 'x'
            -> state -1    if error
            -> state -2    if finished */
         case 3:
            state = 2;
            if(*s >= '0' && *s <= '9') result = (result * 10) + (*s - '0');
            else if(*s == 'x' || *s == 'X') state = 4;
            else if(!*s) state = -2;
            else state = -1;
            break;

         /* Required-hexadecimal-character state
            -> state 5     if character is '0'..'F'
            -> state -1    if error */
         case 4:
            state = 5;
            if(*s >= '0' && *s <= '9') result = (result << 4) | (*s - '0');
            else if(*s >= 'a' && *s <= 'f') result = (result << 4) | (*s - 'a' + 10);
            else if(*s >= 'A' && *s <= 'F') result = (result << 4) | (*s - 'A' + 10);
            else state = -1;
            break;

         /* Optional-hexadecimal-character-state
            -> state 5     if character is '0'..'F'
            -> state -1    if error
            -> state -2    if finished */
         case 5:
            if(*s >= '0' && *s <= '9') result = (result << 4) | (*s - '0');
            else if(*s >= 'a' && *s <= 'f') result = (result << 4) | (*s - 'a' + 10);
            else if(*s >= 'A' && *s <= 'F') result = (result << 4) | (*s - 'A' + 10);
            else if(!*s) state = -2;
            else state = -1;
            break;

         /* Required-octal-character state
            -> state 7     if character is '0'..'7'
            -> state -1    if error */
         case 6:
            state = 7;
            if(*s >= '0' && *s <= '7') result = (result << 3) | (*s - '0');
            else state = -1;
            break;

         /* Optional-octal-character-state
            -> state 7     if character is '0'..'7'
            -> state -1    if error
            -> state -2    if finished */
         case 7:
            if(*s >= '0' && *s <= '7') result = (result << 3) | (*s - '0');
            else if(!*s) state = -2;
            else state = -1;
            break;

         /* Required-binary-character state
            -> state 9     if character is '0'..'1'
            -> state -1    if error */
         case 8:
            state = 9;
            if(*s == '0') result = (result << 1);
            else if(*s == '1') result = (result << 1) | 1;
            else state = -1;
            break;

         /* Optional-binary-character-state
            -> state 9     if character is '0'..'1'
            -> state -1    if error
            -> state -2    if finished */
         case 9:
            if(*s == '0') result = (result << 1);
            else if(*s == '1') result = (result << 1) | 1;
            else if(!*s) state = -2;
            else state = -1;
            break;

      }
      s++;
      if(succ) (*succ)++;
   }

   if(state == -1) return(0);
   if(!possign) result = -result;
   if(succ) *succ = 0;
   return(result);

}
