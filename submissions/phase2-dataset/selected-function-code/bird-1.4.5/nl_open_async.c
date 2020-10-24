static void
nl_open_async(void)
{
  sock *sk;
  struct sockaddr_nl sa;
  int fd;

  if (nl_async_sk)
    return;

  DBG("KRT: Opening async netlink socket\n");

  fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  if (fd < 0)
    {
      log(L_ERR "Unable to open asynchronous rtnetlink socket: %m");
      return;
    }

  bzero(&sa, sizeof(sa));
  sa.nl_family = AF_NETLINK;
#ifdef IPV6
  sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV6_IFADDR | RTMGRP_IPV6_ROUTE;
#else
  sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV4_ROUTE;
#endif
  if (bind(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0)
    {
      log(L_ERR "Unable to bind asynchronous rtnetlink socket: %m");
      close(fd);
      return;
    }

  nl_async_rx_buffer = xmalloc(NL_RX_SIZE);

  sk = nl_async_sk = sk_new(krt_pool);
  sk->type = SK_MAGIC;
  sk->rx_hook = nl_async_hook;
  sk->fd = fd;
  if (sk_open(sk) < 0)
    bug("Netlink: sk_open failed");
}
