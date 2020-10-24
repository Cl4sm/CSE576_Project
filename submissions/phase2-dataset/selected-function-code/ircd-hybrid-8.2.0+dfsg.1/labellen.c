labellen(const unsigned char *lp)
{                               
  int bitlen;
  unsigned char l = *lp;

  if ((l & NS_CMPRSFLGS) == NS_CMPRSFLGS)
  {
    /* Should be avoided by the caller */
    return -1;
  }

  if ((l & NS_CMPRSFLGS) == NS_TYPE_ELT)
  {
    if (l == DNS_LABELTYPE_BITSTRING)
    {
      if ((bitlen = *(lp + 1)) == 0)
        bitlen = 256;
      return (bitlen + 7 ) / 8 + 1;
    }

    return -1;  /* Unknwon ELT */
  }

  return l;
}
