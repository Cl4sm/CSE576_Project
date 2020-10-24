try_parse_v6_netmask(const char *text, struct irc_ssaddr *addr, int *b)
{
  char c;
  int d[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  int dp = 0;
  int nyble = 4;
  int finsert = -1;
  int bits = 128;
  int deficit = 0;
  short dc[8];
  struct sockaddr_in6 *v6 = (struct sockaddr_in6 *)addr;

  for (const char *p = text; (c = *p); ++p)
  {
    if (IsXDigit(c))
    {
      if (nyble == 0)
        return HM_HOST;
      DigitParse(c);
      d[dp] |= c << (4 * --nyble);
    }
    else if (c == ':')
    {
      if (p > text && *(p - 1) == ':')
      {
        if (finsert >= 0)
          return HM_HOST;
        finsert = dp;
      }
      else
      {
        /* If there were less than 4 hex digits, e.g. :ABC: shift right
         * so we don't interpret it as ABC0 -A1kmm */
        d[dp] = d[dp] >> 4 * nyble;
        nyble = 4;
        if (++dp >= 8)
          return HM_HOST;
      }
    }
    else if (c == '*')
    {
      /* * must be last, and * is ambiguous if there is a ::... -A1kmm */
      if (finsert >= 0 || *(p + 1) || dp == 0 || *(p - 1) != ':')
        return HM_HOST;
      bits = dp * 16;
    }
    else if (c == '/')
    {
      char *after;

      d[dp] = d[dp] >> 4 * nyble;
      ++dp;
      bits = strtoul(p + 1, &after, 10);

      if (bits < 0 || *after)
        return HM_HOST;
      if (bits > dp * 4 && !(finsert >= 0 && bits <= 128))
        return HM_HOST;
      break;
    }
    else
      return HM_HOST;
  }

  d[dp] = d[dp] >> 4 * nyble;

  if (c == 0)
    ++dp;
  if (finsert < 0 && bits == 0)
    bits = dp * 16;

  /* How many words are missing? -A1kmm */
  deficit = bits / 16 + ((bits % 16) ? 1 : 0) - dp;

  /* Now fill in the gaps(from ::) in the copied table... -A1kmm */
  for (dp = 0, nyble = 0; dp < 8; ++dp)
  {
    if (nyble == finsert && deficit)
    {
      dc[dp] = 0;
      deficit--;
    }
    else
      dc[dp] = d[nyble++];
  }

  /* Set unused bits to 0... -A1kmm */
  if (bits < 128 && (bits % 16 != 0))
    dc[bits / 16] &= ~((1 << (15 - bits % 16)) - 1);
  for (dp = bits / 16 + (bits % 16 ? 1 : 0); dp < 8; ++dp)
    dc[dp] = 0;

  /* And assign... -A1kmm */
  if (addr)
    for (dp = 0; dp < 8; ++dp)
      /* The cast is a kludge to make netbsd work. */
      ((unsigned short *)&v6->sin6_addr)[dp] = htons(dc[dp]);

  if (b)
    *b = bits;
  return HM_IPV6;
}
