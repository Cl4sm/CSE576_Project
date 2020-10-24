nl_add_multipath(struct nlmsghdr *h, unsigned bufsize, struct mpnh *nh)
{
  unsigned len = sizeof(struct rtattr);
  unsigned pos = NLMSG_ALIGN(h->nlmsg_len);
  char *buf = (char *)h + pos;
  struct rtattr *rt = (void *) buf;
  buf += len;
  
  for (; nh; nh = nh->next)
    {
      len += RTNH_SIZE;
      if (pos + len > bufsize)
	bug("nl_add_multipath: packet buffer overflow");

      add_mpnexthop(buf, nh->gw, nh->iface->index, nh->weight);
      buf += RTNH_SIZE;
    }

  rt->rta_type = RTA_MULTIPATH;
  rt->rta_len = len;
  h->nlmsg_len = pos + len;
}
