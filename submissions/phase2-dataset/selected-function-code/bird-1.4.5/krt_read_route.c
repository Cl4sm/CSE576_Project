static void
krt_read_route(struct ks_msg *msg, struct krt_proto *p, int scan)
{
  /* p is NULL iff KRT_SHARED_SOCKET and !scan */

  rte *e;
  net *net;
  sockaddr dst, gate, mask;
  ip_addr idst, igate, imask;
  void *body = (char *)msg->buf;
  int new = (msg->rtm.rtm_type == RTM_ADD);
  char *errmsg = "KRT: Invalid route received";
  int flags = msg->rtm.rtm_flags;
  int addrs = msg->rtm.rtm_addrs;
  int src;
  byte src2;

  if (!(flags & RTF_UP) && scan)
    SKIP("not up in scan\n");

  if (!(flags & RTF_DONE) && !scan)
    SKIP("not done in async\n");

  if (flags & RTF_LLINFO)
    SKIP("link-local\n");

#ifdef KRT_SHARED_SOCKET
  if (!scan)
  {
    int table_id = msg->rtm.rtm_tableid;
    p = (table_id < KRT_MAX_TABLES) ? krt_table_map[table_id] : NULL;

    if (!p)
      SKIP("unknown table id %d\n", table_id);
  }
#endif

  GETADDR(&dst, RTA_DST);
  GETADDR(&gate, RTA_GATEWAY);
  GETADDR(&mask, RTA_NETMASK);

  if (dst.sa.sa_family != BIRD_AF)
    SKIP("invalid DST");

  idst  = ipa_from_sa(&dst);
  imask = ipa_from_sa(&mask);
  igate = (gate.sa.sa_family == BIRD_AF) ? ipa_from_sa(&gate) : IPA_NONE;

  /* We do not test family for RTA_NETMASK, because BSD sends us
     some strange values, but interpreting them as IPv4/IPv6 works */


  int c = ipa_classify_net(idst);
  if ((c < 0) || !(c & IADDR_HOST) || ((c & IADDR_SCOPE_MASK) <= SCOPE_LINK))
    SKIP("strange class/scope\n");

  int pxlen = (flags & RTF_HOST) ? MAX_PREFIX_LENGTH : ipa_mklen(imask);
  if (pxlen < 0)
    { log(L_ERR "%s (%I) - netmask %I", errmsg, idst, imask); return; }

  if ((flags & RTF_GATEWAY) && ipa_zero(igate))
    { log(L_ERR "%s (%I/%d) - missing gateway", errmsg, idst, pxlen); return; }

  u32 self_mask = RTF_PROTO1;
  u32 alien_mask = RTF_STATIC | RTF_PROTO1 | RTF_GATEWAY;

  src2 = (flags & RTF_STATIC) ? 1 : 0;
  src2 |= (flags & RTF_PROTO1) ? 2 : 0;

#ifdef RTF_PROTO2
  alien_mask |= RTF_PROTO2;
  src2 |= (flags & RTF_PROTO2) ? 4 : 0;
#endif

#ifdef RTF_PROTO3
  alien_mask |= RTF_PROTO3;
  src2 |= (flags & RTF_PROTO3) ? 8 : 0;
#endif

#ifdef RTF_REJECT
  alien_mask |= RTF_REJECT;
#endif

#ifdef RTF_BLACKHOLE
  alien_mask |= RTF_BLACKHOLE;
#endif

  if (flags & (RTF_DYNAMIC | RTF_MODIFIED))
    src = KRT_SRC_REDIRECT;
  else if (flags & self_mask)
    {
      if (!scan)
	SKIP("echo\n");
      src = KRT_SRC_BIRD;
    }
  else if (flags & alien_mask)
    src = KRT_SRC_ALIEN;
  else
    src = KRT_SRC_KERNEL;

  net = net_get(p->p.table, idst, pxlen);

  rta a = {
    .src = p->p.main_source,
    .source = RTS_INHERIT,
    .scope = SCOPE_UNIVERSE,
    .cast = RTC_UNICAST
  };

  /* reject/blackhole routes have also set RTF_GATEWAY,
     we wil check them first. */

#ifdef RTF_REJECT
  if(flags & RTF_REJECT) {
    a.dest = RTD_UNREACHABLE;
    goto done;
  }
#endif

#ifdef RTF_BLACKHOLE
  if(flags & RTF_BLACKHOLE) {
    a.dest = RTD_BLACKHOLE;
    goto done;
  }
#endif

  a.iface = if_find_by_index(msg->rtm.rtm_index);
  if (!a.iface)
    {
      log(L_ERR "KRT: Received route %I/%d with unknown ifindex %u",
	  net->n.prefix, net->n.pxlen, msg->rtm.rtm_index);
      return;
    }

  if (flags & RTF_GATEWAY)
  {
    neighbor *ng;
    a.dest = RTD_ROUTER;
    a.gw = igate;

#ifdef IPV6
    /* Clean up embedded interface ID returned in link-local address */
    if (ipa_has_link_scope(a.gw))
      _I0(a.gw) = 0xfe800000;
#endif

    ng = neigh_find2(&p->p, &a.gw, a.iface, 0);
    if (!ng || (ng->scope == SCOPE_HOST))
      {
	/* Ignore routes with next-hop 127.0.0.1, host routes with such
	   next-hop appear on OpenBSD for address aliases. */
        if (ipa_classify(a.gw) == (IADDR_HOST | SCOPE_HOST))
          return;

	log(L_ERR "KRT: Received route %I/%d with strange next-hop %I",
	    net->n.prefix, net->n.pxlen, a.gw);
	return;
      }
  }
  else
    a.dest = RTD_DEVICE;

 done:
  e = rte_get_temp(&a);
  e->net = net;
  e->u.krt.src = src;
  e->u.krt.proto = src2;

  /* These are probably too Linux-specific */
  e->u.krt.type = 0;
  e->u.krt.metric = 0;

  if (scan)
    krt_got_route(p, e);
  else
    krt_got_route_async(p, e, new);
}
