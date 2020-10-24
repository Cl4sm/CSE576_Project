ipv4_classify(u32 a)
{
  u32 b = a >> 24U;

  if (b && b <= 0xdf)
    {
      if (b == 0x7f)
	return IADDR_HOST | SCOPE_HOST;
      else if (b == 0x0a ||
	       (a & 0xffff0000) == 0xc0a80000 ||
	       (a & 0xfff00000) == 0xac100000)
	return IADDR_HOST | SCOPE_SITE;
      else
	return IADDR_HOST | SCOPE_UNIVERSE;
    }
  if (b >= 0xe0 && b <= 0xef)
    return IADDR_MULTICAST | SCOPE_UNIVERSE;
  if (a == 0xffffffff)
    return IADDR_BROADCAST | SCOPE_LINK;
  return IADDR_INVALID;
}
