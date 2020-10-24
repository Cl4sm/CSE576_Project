int strcomp(const char *s, const char *d) {
/* String-Compare-Nullsafe
   This is a null-pointer-safe version of strcmp. This will return 0
   if both pointers are NULL.  This function considers a NULL pointer
   to be "less than" any valid string, including the empty string.  */

   int result;

   /* Handle trivial equality and NULL cases */
   if(s == d) return(0);
   if(s == NULL) return(-1);
   if(d == NULL) return(+1);

   /* At this point, neither s nor d are NULL */
   result = STRCOMP(s, d);
   if(result == 0) return(0);
   if(result < 0) return(-1);
   return(+1);

}
