void
remove_ipv6_mapping(struct irc_ssaddr *addr)
{
  if (addr->ss.ss_family == AF_INET6)
  {
    if (IN6_IS_ADDR_V4MAPPED(&((struct sockaddr_in6 *)addr)->sin6_addr))
    {
      struct sockaddr_in6 v6;
      struct sockaddr_in *v4 = (struct sockaddr_in *)addr;

      memcpy(&v6, addr, sizeof(v6));
      memset(v4, 0, sizeof(struct sockaddr_in));
      memcpy(&v4->sin_addr, &v6.sin6_addr.s6_addr[12], sizeof(v4->sin_addr));

      addr->ss.ss_family = AF_INET;
      addr->ss_len = sizeof(struct sockaddr_in);
    }
    else
      addr->ss_len = sizeof(struct sockaddr_in6);
  }
  else
    addr->ss_len = sizeof(struct sockaddr_in);
}
