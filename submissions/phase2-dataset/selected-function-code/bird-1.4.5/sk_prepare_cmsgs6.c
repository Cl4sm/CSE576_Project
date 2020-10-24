static inline void
sk_prepare_cmsgs6(sock *s, struct msghdr *msg, void *cbuf, size_t cbuflen)
{
  struct cmsghdr *cm;
  struct in6_pktinfo *pi;
  int controllen = 0;

  msg->msg_control = cbuf;
  msg->msg_controllen = cbuflen;

  cm = CMSG_FIRSTHDR(msg);
  cm->cmsg_level = SOL_IPV6;
  cm->cmsg_type = IPV6_PKTINFO;
  cm->cmsg_len = CMSG_LEN(sizeof(*pi));
  controllen += CMSG_SPACE(sizeof(*pi));

  pi = (struct in6_pktinfo *) CMSG_DATA(cm);
  pi->ipi6_ifindex = s->iface ? s->iface->index : 0;
  pi->ipi6_addr = ipa_to_in6(s->saddr);

  msg->msg_controllen = controllen;
}
