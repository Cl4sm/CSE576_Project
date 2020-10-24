irc_ns_name_skip(const unsigned char **ptrptr, const unsigned char *eom)
{
  const unsigned char *cp;
  unsigned int n;
  int l;

  cp = *ptrptr;

  while (cp < eom && (n = *cp++) != 0)
  {
    /* Check for indirection. */
    switch (n & NS_CMPRSFLGS)
    {
      case 0:  /* Normal case, n == len */
        cp += n;
        continue;
      case NS_TYPE_ELT:  /* EDNS0 extended label */
        if ((l = labellen(cp - 1)) < 0)
        {
          errno = EMSGSIZE;  /* XXX */
          return -1;
        }

        cp += l;
        continue;
      case NS_CMPRSFLGS:  /* Indirection */
        cp++;
        break;
      default:  /* Illegal type */
        errno = EMSGSIZE;
        return -1;
    }

    break;
  }

  if (cp > eom)
  {
    errno = EMSGSIZE;
    return -1;
  }

  *ptrptr = cp;
  return 0;
}
