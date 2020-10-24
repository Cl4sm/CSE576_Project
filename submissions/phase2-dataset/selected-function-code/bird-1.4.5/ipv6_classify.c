int
ipv6_classify(ip_addr *a)
{
  u32 x = a->addr[0];

  if ((x & 0xe0000000) == 0x20000000)		/* 2000::/3  Aggregatable Global Unicast Address */
    return IADDR_HOST | SCOPE_UNIVERSE;
  if ((x & 0xffc00000) == 0xfe800000)		/* fe80::/10 Link-Local Address */
    return IADDR_HOST | SCOPE_LINK;
  if ((x & 0xffc00000) == 0xfec00000)		/* fec0::/10 Site-Local Address */
    return IADDR_HOST | SCOPE_SITE;
  if ((x & 0xfe000000) == 0xfc000000)		/* fc00::/7  Unique Local Unicast Address (RFC 4193) */
    return IADDR_HOST | SCOPE_SITE;
  if ((x & 0xff000000) == 0xff000000)		/* ff00::/8  Multicast Address */
    {
      unsigned int scope = (x >> 16) & 0x0f;
      switch (scope)
	{
	case 1:	 return IADDR_MULTICAST | SCOPE_HOST;
	case 2:  return IADDR_MULTICAST | SCOPE_LINK;
	case 5:  return IADDR_MULTICAST | SCOPE_SITE;
	case 8:  return IADDR_MULTICAST | SCOPE_ORGANIZATION;
	case 14: return IADDR_MULTICAST | SCOPE_UNIVERSE;
	default: return IADDR_MULTICAST | SCOPE_UNDEFINED;
	}
    }
  if (!x && !a->addr[1] && !a->addr[2])
    {
      u32 y = a->addr[3];
      if (y == 1)
	return IADDR_HOST | SCOPE_HOST;		/* Loopback address */
      /* IPv4 compatible addresses */
      if (y >= 0x7f000000 && y < 0x80000000)
	return IADDR_HOST | SCOPE_HOST;
      if ((y & 0xff000000) == 0x0a000000 ||
	  (y & 0xffff0000) == 0xc0a80000 ||
	  (y & 0xfff00000) == 0xac100000)
	return IADDR_HOST | SCOPE_SITE;
      if (y >= 0x01000000 && y < 0xe0000000)
	return IADDR_HOST | SCOPE_UNIVERSE;
    }
  return IADDR_HOST | SCOPE_UNDEFINED;
}
