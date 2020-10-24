static inline int
sk_recvmsg(sock *s)
{
  struct iovec iov = {s->rbuf, s->rbsize};
  byte cmsg_buf[CMSG_RX_SPACE];
  sockaddr src;

  struct msghdr msg = {
    .msg_name = &src.sa,
    .msg_namelen = sizeof(src), // XXXX ??
    .msg_iov = &iov,
    .msg_iovlen = 1,
    .msg_control = cmsg_buf,
    .msg_controllen = sizeof(cmsg_buf),
    .msg_flags = 0
  };

  int rv = recvmsg(s->fd, &msg, 0);
  if (rv < 0)
    return rv;

  //ifdef IPV4
  //  if (cf_type == SK_IP)
  //    rv = ipv4_skip_header(pbuf, rv);
  //endif

  sockaddr_read(&src, s->af, &s->faddr, NULL, &s->fport);
  sk_process_cmsgs(s, &msg);

  if (msg.msg_flags & MSG_TRUNC)
    s->flags |= SKF_TRUNCATED;
  else
    s->flags &= ~SKF_TRUNCATED;

  return rv;
}
