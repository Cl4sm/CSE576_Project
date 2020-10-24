nl_parse_multipath(struct krt_proto *p, struct rtattr *ra)
{
  /* Temporary buffer for multicast nexthops */
  static struct mpnh *nh_buffer;
  static int nh_buf_size;	/* in number of structures */
  static int nh_buf_used;

  struct rtattr *a[RTA_CACHEINFO+1];
  struct rtnexthop *nh = RTA_DATA(ra);
  struct mpnh *rv, *first, **last;
  int len = RTA_PAYLOAD(ra);

  first = NULL;
  last = &first;
  nh_buf_used = 0;

  while (len)
    {
      /* Use RTNH_OK(nh,len) ?? */
      if ((len < sizeof(*nh)) || (len < nh->rtnh_len))
	return NULL;

      if (nh_buf_used == nh_buf_size)
      {
	nh_buf_size = nh_buf_size ? (nh_buf_size * 2) : 4;
	nh_buffer = xrealloc(nh_buffer, nh_buf_size * sizeof(struct mpnh));
      }
      *last = rv = nh_buffer + nh_buf_used++;
      rv->next = NULL;
      last = &(rv->next);

      rv->weight = nh->rtnh_hops;
      rv->iface = if_find_by_index(nh->rtnh_ifindex);
      if (!rv->iface)
	return NULL;

      /* Nonexistent RTNH_PAYLOAD ?? */
      nl_attr_len = nh->rtnh_len - RTNH_LENGTH(0);
      nl_parse_attrs(RTNH_DATA(nh), a, sizeof(a));
      if (a[RTA_GATEWAY])
	{
	  if (RTA_PAYLOAD(a[RTA_GATEWAY]) != sizeof(ip_addr))
	    return NULL;

	  memcpy(&rv->gw, RTA_DATA(a[RTA_GATEWAY]), sizeof(ip_addr));
	  ipa_ntoh(rv->gw);

	  neighbor *ng = neigh_find2(&p->p, &rv->gw, rv->iface,
				     (nh->rtnh_flags & RTNH_F_ONLINK) ? NEF_ONLINK : 0);
	  if (!ng || (ng->scope == SCOPE_HOST))
	    return NULL;
	}
      else
	return NULL;

      len -= NLMSG_ALIGN(nh->rtnh_len);
      nh = RTNH_NEXT(nh);
    }

  return first;
}
