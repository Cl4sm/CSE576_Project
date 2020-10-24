nl_parse_route(struct nlmsghdr *h, int scan)
{
  struct krt_proto *p;
  struct rtmsg *i;
  struct rtattr *a[RTA_CACHEINFO+1];
  int new = h->nlmsg_type == RTM_NEWROUTE;

  ip_addr dst = IPA_NONE;
  u32 oif = ~0;
  int src;

  if (!(i = nl_checkin(h, sizeof(*i))) || !nl_parse_attrs(RTM_RTA(i), a, sizeof(a)))
    return;
  if (i->rtm_family != BIRD_AF)
    return;
  if ((a[RTA_DST] && RTA_PAYLOAD(a[RTA_DST]) != sizeof(ip_addr)) ||
#ifdef IPV6
      (a[RTA_IIF] && RTA_PAYLOAD(a[RTA_IIF]) != 4) ||
#endif
      (a[RTA_OIF] && RTA_PAYLOAD(a[RTA_OIF]) != 4) ||
      (a[RTA_GATEWAY] && RTA_PAYLOAD(a[RTA_GATEWAY]) != sizeof(ip_addr)) ||
      (a[RTA_PRIORITY] && RTA_PAYLOAD(a[RTA_PRIORITY]) != 4) ||
      (a[RTA_PREFSRC] && RTA_PAYLOAD(a[RTA_PREFSRC]) != sizeof(ip_addr)) ||
      (a[RTA_FLOW] && RTA_PAYLOAD(a[RTA_FLOW]) != 4))
    {
      log(L_ERR "KRT: Malformed message received");
      return;
    }

  if (a[RTA_DST])
    {
      memcpy(&dst, RTA_DATA(a[RTA_DST]), sizeof(dst));
      ipa_ntoh(dst);
    }

  if (a[RTA_OIF])
    memcpy(&oif, RTA_DATA(a[RTA_OIF]), sizeof(oif));

  p = nl_table_map[i->rtm_table];	/* Do we know this table? */
  DBG("KRT: Got %I/%d, type=%d, oif=%d, table=%d, prid=%d, proto=%s\n", dst, i->rtm_dst_len, i->rtm_type, oif, i->rtm_table, i->rtm_protocol, p ? p->p.name : "(none)");
  if (!p)
    SKIP("unknown table %d\n", i->rtm_table);


#ifdef IPV6
  if (a[RTA_IIF])
    SKIP("IIF set\n");
#else
  if (i->rtm_tos != 0)			/* We don't support TOS */
    SKIP("TOS %02x\n", i->rtm_tos);
#endif

  if (scan && !new)
    SKIP("RTM_DELROUTE in scan\n");

  int c = ipa_classify_net(dst);
  if ((c < 0) || !(c & IADDR_HOST) || ((c & IADDR_SCOPE_MASK) <= SCOPE_LINK))
    SKIP("strange class/scope\n");

  // ignore rtm_scope, it is not a real scope
  // if (i->rtm_scope != RT_SCOPE_UNIVERSE)
  //   SKIP("scope %u\n", i->rtm_scope);

  switch (i->rtm_protocol)
    {
    case RTPROT_UNSPEC:
      SKIP("proto unspec\n");

    case RTPROT_REDIRECT:
      src = KRT_SRC_REDIRECT;
      break;

    case RTPROT_KERNEL:
      src = KRT_SRC_KERNEL;
      return;

    case RTPROT_BIRD:
      if (!scan)
	SKIP("echo\n");
      src = KRT_SRC_BIRD;
      break;

    case RTPROT_BOOT:
    default:
      src = KRT_SRC_ALIEN;
    }

  net *net = net_get(p->p.table, dst, i->rtm_dst_len);

  rta ra = {
    .src= p->p.main_source,
    .source = RTS_INHERIT,
    .scope = SCOPE_UNIVERSE,
    .cast = RTC_UNICAST
  };

  switch (i->rtm_type)
    {
    case RTN_UNICAST:

      if (a[RTA_MULTIPATH])
	{
	  ra.dest = RTD_MULTIPATH;
	  ra.nexthops = nl_parse_multipath(p, a[RTA_MULTIPATH]);
	  if (!ra.nexthops)
	    {
	      log(L_ERR "KRT: Received strange multipath route %I/%d",
		  net->n.prefix, net->n.pxlen);
	      return;
	    }
	    
	  break;
	}

      ra.iface = if_find_by_index(oif);
      if (!ra.iface)
	{
	  log(L_ERR "KRT: Received route %I/%d with unknown ifindex %u",
	      net->n.prefix, net->n.pxlen, oif);
	  return;
	}

      if (a[RTA_GATEWAY])
	{
	  neighbor *ng;
	  ra.dest = RTD_ROUTER;
	  memcpy(&ra.gw, RTA_DATA(a[RTA_GATEWAY]), sizeof(ra.gw));
	  ipa_ntoh(ra.gw);

#ifdef IPV6
	  /* Silently skip strange 6to4 routes */
	  if (ipa_in_net(ra.gw, IPA_NONE, 96))
	    return;
#endif

	  ng = neigh_find2(&p->p, &ra.gw, ra.iface,
			   (i->rtm_flags & RTNH_F_ONLINK) ? NEF_ONLINK : 0);
	  if (!ng || (ng->scope == SCOPE_HOST))
	    {
	      log(L_ERR "KRT: Received route %I/%d with strange next-hop %I",
		  net->n.prefix, net->n.pxlen, ra.gw);
	      return;
	    }
	}
      else
	{
	  ra.dest = RTD_DEVICE;
	}

      break;
    case RTN_BLACKHOLE:
      ra.dest = RTD_BLACKHOLE;
      break;
    case RTN_UNREACHABLE:
      ra.dest = RTD_UNREACHABLE;
      break;
    case RTN_PROHIBIT:
      ra.dest = RTD_PROHIBIT;
      break;
    /* FIXME: What about RTN_THROW? */
    default:
      SKIP("type %d\n", i->rtm_type);
      return;
    }

  rte *e = rte_get_temp(&ra);
  e->net = net;
  e->u.krt.src = src;
  e->u.krt.proto = i->rtm_protocol;
  e->u.krt.type = i->rtm_type;

  if (a[RTA_PRIORITY])
    memcpy(&e->u.krt.metric, RTA_DATA(a[RTA_PRIORITY]), sizeof(e->u.krt.metric)); 
  else
    e->u.krt.metric = 0;

  if (a[RTA_PREFSRC])
    {
      ip_addr ps;
      memcpy(&ps, RTA_DATA(a[RTA_PREFSRC]), sizeof(ps));
      ipa_ntoh(ps);

      ea_list *ea = alloca(sizeof(ea_list) + sizeof(eattr));
      ea->next = ra.eattrs;
      ra.eattrs = ea;
      ea->flags = EALF_SORTED;
      ea->count = 1;
      ea->attrs[0].id = EA_KRT_PREFSRC;
      ea->attrs[0].flags = 0;
      ea->attrs[0].type = EAF_TYPE_IP_ADDRESS;
      ea->attrs[0].u.ptr = alloca(sizeof(struct adata) + sizeof(ps));
      ea->attrs[0].u.ptr->length = sizeof(ps);
      memcpy(ea->attrs[0].u.ptr->data, &ps, sizeof(ps));
    }

  if (a[RTA_FLOW])
    {
      ea_list *ea = alloca(sizeof(ea_list) + sizeof(eattr));
      ea->next = ra.eattrs;
      ra.eattrs = ea;
      ea->flags = EALF_SORTED;
      ea->count = 1;
      ea->attrs[0].id = EA_KRT_REALM;
      ea->attrs[0].flags = 0;
      ea->attrs[0].type = EAF_TYPE_INT;
      memcpy(&ea->attrs[0].u.data, RTA_DATA(a[RTA_FLOW]), 4);
    }

  if (scan)
    krt_got_route(p, e);
  else
    krt_got_route_async(p, e, new);
}
