sizea getnumargs(const char *s, char sep) {
/* Get-Number-of-Arguments
   total number of arguments in a cmd string (assuming '|' separates
   arguments) You can optionally specify the "sep" character to
   denote your own character to separate arguments. Note, this
   command will stop processing when it reaches a null character.
   This command will return 0 if the string is empty, or 1 if no
   separator characters are found. */

   sizea r = 0;

   if(s != NULL && *s != '\0') {
      r = 1;
      while(*s != '\0') {
         if(*s == sep) ++r;
         ++s;
      }
   }
   return(r);

}
