static int
try_parse_v4_netmask(const char *text, struct irc_ssaddr *addr, int *b)
{
  const char *digits[4];
  unsigned char addb[4];
  int n = 0, bits = 0;
  char c;
  struct sockaddr_in *v4 = (struct sockaddr_in *)addr;

  digits[n++] = text;

  for (const char *p = text; (c = *p); ++p)
  {
    if (c >= '0' && c <= '9')   /* empty */
      ;
    else if (c == '.')
    {
      if (n >= 4)
        return HM_HOST;

      digits[n++] = p + 1;
    }
    else if (c == '*')
    {
      if (*(p + 1) || n == 0 || *(p - 1) != '.')
        return HM_HOST;

      bits = (n - 1) * 8;
      break;
    }
    else if (c == '/')
    {
      char *after;
      bits = strtoul(p + 1, &after, 10);

      if (bits < 0 || *after)
        return HM_HOST;
      if (bits > n * 8)
        return HM_HOST;

      break;
    }
    else
      return HM_HOST;
  }

  if (n < 4 && bits == 0)
    bits = n * 8;
  if (bits)
    while (n < 4)
      digits[n++] = "0";

  for (n = 0; n < 4; ++n)
    addb[n] = strtoul(digits[n], NULL, 10);

  if (bits == 0)
    bits = 32;

  /* Set unused bits to 0... -A1kmm */
  if (bits < 32 && bits % 8)
    addb[bits / 8] &= ~((1 << (8 - bits % 8)) - 1);
  for (n = bits / 8 + (bits % 8 ? 1 : 0); n < 4; ++n)
    addb[n] = 0;
  if (addr)
    v4->sin_addr.s_addr =
      htonl(addb[0] << 24 | addb[1] << 16 | addb[2] << 8 | addb[3]);
  if (b)
    *b = bits;
  return HM_IPV4;
}
