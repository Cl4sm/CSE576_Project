sockaddr_read(sockaddr *sa, int af, ip_addr *a, struct iface **ifa, uint *port)
{
  if (sa->sa.sa_family != af)
    goto fail;

  if (af == AF_INET)
    sockaddr_read4((struct sockaddr_in *) sa, a, ifa, port);
  else if (af == AF_INET6)
    sockaddr_read6((struct sockaddr_in6 *) sa, a, ifa, port);
  else
    goto fail;

  return 0;

 fail:
  *a = IPA_NONE;
  *port = 0;
  return -1;
}
