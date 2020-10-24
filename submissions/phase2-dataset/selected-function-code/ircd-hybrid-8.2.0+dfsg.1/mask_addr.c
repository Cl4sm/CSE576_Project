void
mask_addr(struct irc_ssaddr *ip, int bits)
{
  int mask;
  struct sockaddr_in6 *v6_base_ip;
  int i, m, n;
  struct sockaddr_in *v4_base_ip;

  if (ip->ss.ss_family == AF_INET)
  {
    uint32_t tmp = 0;
    v4_base_ip = (struct sockaddr_in *)ip;

    mask = ~((1 << (32 - bits)) - 1);
    tmp = ntohl(v4_base_ip->sin_addr.s_addr);
    v4_base_ip->sin_addr.s_addr = htonl(tmp & mask);
  }
  else
  {
    n = bits / 8;
    m = bits % 8;
    v6_base_ip = (struct sockaddr_in6 *)ip;

    mask = ~((1 << (8 - m)) -1 );
    v6_base_ip->sin6_addr.s6_addr[n] = v6_base_ip->sin6_addr.s6_addr[n] & mask;

    for (i = n + 1; i < 16; i++)
      v6_base_ip->sin6_addr.s6_addr[i] = 0;
  }
}
