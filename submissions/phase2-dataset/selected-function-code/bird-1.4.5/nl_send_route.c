static int
nl_send_route(struct krt_proto *p, rte *e, struct ea_list *eattrs, int new)
{
  eattr *ea;
  net *net = e->net;
  rta *a = e->attrs;
  struct {
    struct nlmsghdr h;
    struct rtmsg r;
    char buf[128 + nh_bufsize(a->nexthops)];
  } r;

  DBG("nl_send_route(%I/%d,new=%d)\n", net->n.prefix, net->n.pxlen, new);

  bzero(&r.h, sizeof(r.h));
  bzero(&r.r, sizeof(r.r));
  r.h.nlmsg_type = new ? RTM_NEWROUTE : RTM_DELROUTE;
  r.h.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
  r.h.nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK | (new ? NLM_F_CREATE|NLM_F_EXCL : 0);

  r.r.rtm_family = BIRD_AF;
  r.r.rtm_dst_len = net->n.pxlen;
  r.r.rtm_tos = 0;
  r.r.rtm_table = KRT_CF->sys.table_id;
  r.r.rtm_protocol = RTPROT_BIRD;
  r.r.rtm_scope = RT_SCOPE_UNIVERSE;
  nl_add_attr_ipa(&r.h, sizeof(r), RTA_DST, net->n.prefix);

  u32 metric = 0;
  if (new && e->attrs->source == RTS_INHERIT)
    metric = e->u.krt.metric;
  if (ea = ea_find(eattrs, EA_KRT_METRIC))
    metric = ea->u.data;
  if (metric != 0)
    nl_add_attr_u32(&r.h, sizeof(r), RTA_PRIORITY, metric);

  if (ea = ea_find(eattrs, EA_KRT_PREFSRC))
    nl_add_attr_ipa(&r.h, sizeof(r), RTA_PREFSRC, *(ip_addr *)ea->u.ptr->data);

  if (ea = ea_find(eattrs, EA_KRT_REALM))
    nl_add_attr_u32(&r.h, sizeof(r), RTA_FLOW, ea->u.data);

  /* a->iface != NULL checked in krt_capable() for router and device routes */

  switch (a->dest)
    {
    case RTD_ROUTER:
      r.r.rtm_type = RTN_UNICAST;
      nl_add_attr_u32(&r.h, sizeof(r), RTA_OIF, a->iface->index);
      nl_add_attr_ipa(&r.h, sizeof(r), RTA_GATEWAY, a->gw);
      break;
    case RTD_DEVICE:
      r.r.rtm_type = RTN_UNICAST;
      nl_add_attr_u32(&r.h, sizeof(r), RTA_OIF, a->iface->index);
      break;
    case RTD_BLACKHOLE:
      r.r.rtm_type = RTN_BLACKHOLE;
      break;
    case RTD_UNREACHABLE:
      r.r.rtm_type = RTN_UNREACHABLE;
      break;
    case RTD_PROHIBIT:
      r.r.rtm_type = RTN_PROHIBIT;
      break;
    case RTD_MULTIPATH:
      r.r.rtm_type = RTN_UNICAST;
      nl_add_multipath(&r.h, sizeof(r), a->nexthops);
      break;
    default:
      bug("krt_capable inconsistent with nl_send_route");
    }

  return nl_exchange(&r.h);
}
