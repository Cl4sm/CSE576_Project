irc_dn_find(const unsigned char *domain, const unsigned char *msg,
            const unsigned char * const *dnptrs,
            const unsigned char * const *lastdnptr)
{
  const unsigned char *dn, *cp, *sp;
  const unsigned char * const *cpp;
  unsigned int n;

  for (cpp = dnptrs; cpp < lastdnptr; cpp++)
  {
    sp = *cpp;
    /*
     * terminate search on:
     * root label
     * compression pointer
     * unusable offset
     */
    while (*sp != 0 && (*sp & NS_CMPRSFLGS) == 0 &&
           (sp - msg) < 0x4000) {
      dn = domain;
      cp = sp;
      while ((n = *cp++) != 0) {
        /*
         * check for indirection
         */
        switch (n & NS_CMPRSFLGS) {
        case 0:   /* normal case, n == len */
          n = labellen(cp - 1); /* XXX */

          if (n != *dn++)
            goto next;

          for ((void)NULL; n > 0; n--)
            if (mklower(*dn++) !=
                mklower(*cp++))
              goto next;
          /* Is next root for both ? */
          if (*dn == '\0' && *cp == '\0')
            return (sp - msg);
          if (*dn)
            continue;
          goto next;
        case NS_CMPRSFLGS:  /* indirection */
          cp = msg + (((n & 0x3f) << 8) | *cp);
          break;

        default:  /* illegal type */
          errno = EMSGSIZE;
          return (-1);
        }
      }
 next: ;
      sp += *sp + 1;
    }
  }
  errno = ENOENT;
  return (-1);
} /*2*/
