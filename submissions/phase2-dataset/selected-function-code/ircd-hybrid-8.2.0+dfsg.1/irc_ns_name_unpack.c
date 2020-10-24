static int
irc_ns_name_unpack(const unsigned char *msg, const unsigned char *eom,
                   const unsigned char *src, unsigned char *dst,
                   size_t dstsiz)
{
  const unsigned char *srcp, *dstlim;
  unsigned char *dstp;
  int n, len, checked, l;

  len = -1;
  checked = 0;
  dstp = dst;
  srcp = src;
  dstlim = dst + dstsiz;

  if (srcp < msg || srcp >= eom)
  {
    errno = EMSGSIZE;
    return -1;
  }

  /* Fetch next label in domain name. */
  while ((n = *srcp++) != 0)
  {
    /* Check for indirection. */
    switch (n & NS_CMPRSFLGS)
    {
      case 0:
      case NS_TYPE_ELT:
        /* Limit checks. */
        if ((l = labellen(srcp - 1)) < 0)
        {
          errno = EMSGSIZE;
          return -1;
        }

        if (dstp + l + 1 >= dstlim || srcp + l >= eom)
        {
          errno = EMSGSIZE;
          return -1;
        }

        checked += l + 1;
        *dstp++ = n;
        memcpy(dstp, srcp, l);

        dstp += l;
        srcp += l;
        break;

      case NS_CMPRSFLGS:
        if (srcp >= eom)
        {
          errno = EMSGSIZE;
          return -1;
        }

        if (len < 0)
          len = srcp - src + 1;
        srcp = msg + (((n & 0x3f) << 8) | (*srcp & 0xff));

        if (srcp < msg || srcp >= eom)
        { 
          /* Out of range. */
          errno = EMSGSIZE;
          return -1;
        }

        checked += 2;

        /*
         * Check for loops in the compressed name;
         * if we've looked at the whole message,
         * there must be a loop.
         */
        if (checked >= eom - msg)
        {
          errno = EMSGSIZE;
          return -1;
        }

        break;

      default:
        errno = EMSGSIZE;
        return -1; /* Flag error */
    }
  }

  *dstp = '\0';

  if (len < 0)
    len = srcp - src;

  return len;
}
