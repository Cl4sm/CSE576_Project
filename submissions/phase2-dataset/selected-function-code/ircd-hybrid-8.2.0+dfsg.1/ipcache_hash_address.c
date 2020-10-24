ipcache_hash_address(const struct irc_ssaddr *addr)
{
  if (addr->ss.ss_family == AF_INET)
  {
    const struct sockaddr_in *v4 = (const struct sockaddr_in *)addr;
    uint32_t hash = 0, ip = ntohl(v4->sin_addr.s_addr);

    hash = ((ip >> 12) + ip) & (IP_HASH_SIZE - 1);
    return hash;
  }
  else
  {
    const struct sockaddr_in6 *v6 = (const struct sockaddr_in6 *)addr;
    uint32_t hash = 0, *ip = (uint32_t *)&v6->sin6_addr.s6_addr;

    hash  = ip[0] ^ ip[3];
    hash ^= hash >> 16;
    hash ^= hash >> 8;
    hash  = hash & (IP_HASH_SIZE - 1);
    return hash;
  }
}
