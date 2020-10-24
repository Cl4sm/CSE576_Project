static inline int
sk_sendmsg(sock *s)
{
  struct iovec iov = {s->tbuf, s->tpos - s->tbuf};
  byte cmsg_buf[CMSG_TX_SPACE];
  sockaddr dst;

  sockaddr_fill(&dst, s->af, s->daddr, s->iface, s->dport);

  struct msghdr msg = {
    .msg_name = &dst.sa,
    .msg_namelen = SA_LEN(dst),
    .msg_iov = &iov,
    .msg_iovlen = 1
  };

#ifdef CONFIG_USE_HDRINCL
  byte hdr[20];
  struct iovec iov2[2] = { {hdr, 20}, iov };

  if (s->flags & SKF_HDRINCL)
  {
    sk_prepare_ip_header(s, hdr, iov.iov_len);
    msg.msg_iov = iov2;
    msg.msg_iovlen = 2;
  }
#endif

  if (s->flags & SKF_PKTINFO)
    sk_prepare_cmsgs(s, &msg, cmsg_buf, sizeof(cmsg_buf));

  return sendmsg(s->fd, &msg, 0);
}
