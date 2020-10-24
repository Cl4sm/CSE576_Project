static void
sk_process_cmsgs(sock *s, struct msghdr *msg)
{
  struct cmsghdr *cm;

  s->laddr = IPA_NONE;
  s->lifindex = 0;
  s->rcv_ttl = -1;

  for (cm = CMSG_FIRSTHDR(msg); cm != NULL; cm = CMSG_NXTHDR(msg, cm))
  {
    if ((cm->cmsg_level == SOL_IP) && sk_is_ipv4(s))
    {
      sk_process_cmsg4_pktinfo(s, cm);
      sk_process_cmsg4_ttl(s, cm);
    }

    if ((cm->cmsg_level == SOL_IPV6) && sk_is_ipv6(s))
    {
      sk_process_cmsg6_pktinfo(s, cm);
      sk_process_cmsg6_ttl(s, cm);
    }
  }
}
