static int
irc_ns_name_pack(const unsigned char *src, unsigned char *dst, int dstsiz,
                 const unsigned char **dnptrs, const unsigned char **lastdnptr)
{
  unsigned char *dstp;
  const unsigned char **cpp, **lpp, *eob, *msg;
  const unsigned char *srcp;
  int n, l, first = 1;

  srcp = src;
  dstp = dst;
  eob = dstp + dstsiz;
  lpp = cpp = NULL;

  if (dnptrs != NULL)
  {
    if ((msg = *dnptrs++))
    {
      for (cpp = dnptrs; *cpp != NULL; cpp++)
        ;
      lpp = cpp;  /* End of list to search */
    }
  }
  else
    msg = NULL;

  /* Make sure the domain we are about to add is legal */
  l = 0;
  do
  {
    int l0;

    n = *srcp;
    if ((n & NS_CMPRSFLGS) == NS_CMPRSFLGS)
    {
      errno = EMSGSIZE;
      return -1;
    }

    if ((l0 = labellen(srcp)) < 0)
    {
      errno = EINVAL;
      return -1;
    }

    l += l0 + 1;
    if (l > NS_MAXCDNAME)
    {
      errno = EMSGSIZE;
      return -1;
    }

    srcp += l0 + 1;
  } while (n != 0);

  /* From here on we need to reset compression pointer array on error */
  srcp = src;
  do
  {
    /* Look to see if we can use pointers. */
    n = *srcp;

    if (n != 0 && msg != NULL)
    {
      l = irc_dn_find(srcp, msg, (const unsigned char *const *)dnptrs,
                      (const unsigned char *const *)lpp);

      if (l >= 0)
      {
        if (dstp + 1 >= eob)
          goto cleanup;

        *dstp++ = (l >> 8) | NS_CMPRSFLGS;
        *dstp++ = l % 256;
        return dstp - dst;
      }

      /* Not found, save it. */
      if (lastdnptr != NULL && cpp < lastdnptr - 1 && (dstp - msg) < 0x4000 && first)
      {
        *cpp++ = dstp;
        *cpp = NULL;
        first = 0;
      }
    }

    /* Copy label to buffer */
    if ((n & NS_CMPRSFLGS) == NS_CMPRSFLGS)
      goto cleanup;  /* Should not happen. */

    n = labellen(srcp);
    if (dstp + 1 + n >= eob)
      goto cleanup;

    memcpy(dstp, srcp, n + 1);
    srcp += n + 1;
    dstp += n + 1;
  } while (n != 0);

  if (dstp > eob)
  {
cleanup:
    if (msg != NULL)
      *lpp = NULL;
    errno = EMSGSIZE;
    return -1;
  }

  return dstp - dst;
}
