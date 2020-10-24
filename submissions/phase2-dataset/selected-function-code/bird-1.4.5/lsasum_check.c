lsasum_check(struct ospf_lsa_header *h, void *body)
{
  u8 *sp, *ep, *p, *q, *b;
  int c0 = 0, c1 = 0;
  int x, y;
  u16 length;

  b = body;
  sp = (char *) h;
  sp += 2; /* Skip Age field */
  length = ntohs(h->length) - 2;
  h->checksum = 0;

  for (ep = sp + length; sp < ep; sp = q)
  {				/* Actually MODX is very large, do we need the for-cyclus? */
    q = sp + MODX;
    if (q > ep)
      q = ep;
    for (p = sp; p < q; p++)
    {
      /* 
       * I count with bytes from header and than from body
       * but if there is no body, it's appended to header
       * (probably checksum in update receiving) and I go on
       * after header
       */
      if ((b == NULL) || (p < (u8 *) (h + 1)))
      {
	c0 += *p;
      }
      else
      {
	c0 += *(b + (p - (u8 *) (h + 1)));
      }

      c1 += c0;
    }
    c0 %= 255;
    c1 %= 255;
  }

  x = (int)((length - LSA_CHECKSUM_OFFSET) * c0 - c1) % 255;
  if (x <= 0)
    x += 255;
  y = 510 - c0 - x;
  if (y > 255)
    y -= 255;

  ((u8 *) & h->checksum)[0] = x;
  ((u8 *) & h->checksum)[1] = y;
  return h->checksum;
}
