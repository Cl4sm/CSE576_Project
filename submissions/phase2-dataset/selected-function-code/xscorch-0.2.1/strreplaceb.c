/* strreplaceb
   Replaces occurrences of src with dst in the string s.  Here, ssize
   is the size of s's buffer; we guarantee that s will terminate with
   NULL after this call, so we may truncate part of a replacement in this.
   If src or s are NULL, nothing happens; if dst is NULL, then occurrences
   of src are removed.  NOTE: we do not scan dst for further replacements
   in this function.  */

   char *s;
   char *p;
   sizea ssize;
   sizea srclen;
   sizea dstlen = 0;
   sizea count = 0;

   /* Sanity checks */
   if(in_s == NULL || src == NULL || in_ssize <= 0 || *src == '\0') return(0);

   /* Prepare to search for occurrences of src */
   s = in_s;
   ssize = in_ssize;
   srclen = STRLEN(src);
   dstlen = STRLENN(dst);

   while((p = strscan(s, src)) != NULL) {
      /* Found an occurrence of src */
      if(ssize <= (sizea)(p - s)) {
         /* We cannot replace this text, it is beyond end of buffer */
         return(count);
      }
      ++count;
      ssize -= p - s;
      if(dst == NULL) {
         /* Delete the source string */
         ssize -= srclen;
         MEMMOVE(p, p + srclen, ssize);
         s = p;
      } else if(dstlen + 1 >= ssize) {
         /* Destination will have to be truncated; tail of s
            will be lost in this operation.  This must appear
            before the check below in case repl string crossed
            the buffer boundary. */
         STRNCPY(p, dst, ssize);
         s += ssize - 1;
         ssize = 1;
      } else if(dstlen <= srclen) {
         /* Tail of s will be moved forward a bit. */
         s = p + dstlen;
         if(dstlen != srclen) MEMMOVE(s, p + srclen, ssize - srclen);
         MEMCPY(p, dst, dstlen);
         ssize -= dstlen;
      } else {
         /* Destination will NOT have to be truncated, but
            since dstlen > srclen, we will have to truncate
            s as we move it.  Also, we need to move s out
            of the way before copying in the dst data.  */
         s = p + dstlen;
         ssize -= srclen + (dstlen - srclen);
         MEMMOVE(s, p + srclen, ssize);
         MEMCPY(p, dst, dstlen);
         s[ssize - 1] = '\0';    /* NULL terminator may have been lost */
      } /* All possible replacement cases */
   } /* Found a string to rewrite */

   /* Return the number of replacements that happened */
   return(count);

}
