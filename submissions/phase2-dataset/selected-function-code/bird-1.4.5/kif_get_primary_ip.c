kif_get_primary_ip(struct iface *i)
{
#ifndef IPV6
  static int fd = -1;
  
  if (fd < 0)
    fd = socket(AF_INET, SOCK_DGRAM, 0);

  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, i->name, IFNAMSIZ);

  int rv = ioctl(fd, SIOCGIFADDR, (char *) &ifr);
  if (rv < 0)
    return NULL;

  ip_addr addr;
  struct sockaddr_in *sin = (struct sockaddr_in *) &ifr.ifr_addr;
  memcpy(&addr, &sin->sin_addr.s_addr, sizeof(ip_addr));
  ipa_ntoh(addr);

  struct ifa *a;
  WALK_LIST(a, i->addrs)
  {
    if (ipa_equal(a->ip, addr))
      return a;
  }
#endif

  return NULL;
}
