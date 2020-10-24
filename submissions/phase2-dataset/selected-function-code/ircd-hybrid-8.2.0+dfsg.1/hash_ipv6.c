static uint32_t
hash_ipv6(const struct irc_ssaddr *addr, int bits)
{
  uint32_t v = 0, n;
  const struct sockaddr_in6 *v6 = (const struct sockaddr_in6 *)addr;

  for (n = 0; n < 16; ++n)
  {
    if (bits >= 8)
    {
      v ^= v6->sin6_addr.s6_addr[n];
      bits -= 8;
    }
    else if (bits)
    {
      v ^= v6->sin6_addr.s6_addr[n] & ~((1 << (8 - bits)) - 1);
      return v & (ATABLE_SIZE - 1);
    }
    else
      return v & (ATABLE_SIZE - 1);
  }

  return v & (ATABLE_SIZE - 1);
}
