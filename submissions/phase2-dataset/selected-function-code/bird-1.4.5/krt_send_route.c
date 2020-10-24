static int
krt_send_route(struct krt_proto *p, int cmd, rte *e)
{
  net *net = e->net;
  rta *a = e->attrs;
  static int msg_seq;
  struct iface *j, *i = a->iface;
  int l;
  struct ks_msg msg;
  char *body = (char *)msg.buf;
  sockaddr gate, mask, dst;
  ip_addr gw;

  DBG("krt-sock: send %I/%d via %I\n", net->n.prefix, net->n.pxlen, a->gw);

  bzero(&msg,sizeof (struct rt_msghdr));
  msg.rtm.rtm_version = RTM_VERSION;
  msg.rtm.rtm_type = cmd;
  msg.rtm.rtm_seq = msg_seq++;
  msg.rtm.rtm_addrs = RTA_DST;
  msg.rtm.rtm_flags = RTF_UP | RTF_PROTO1;

  if (net->n.pxlen == MAX_PREFIX_LENGTH)
    msg.rtm.rtm_flags |= RTF_HOST;
  else
    msg.rtm.rtm_addrs |= RTA_NETMASK;

#ifdef KRT_SHARED_SOCKET
  msg.rtm.rtm_tableid = KRT_CF->sys.table_id;
#endif

#ifdef RTF_REJECT
  if(a->dest == RTD_UNREACHABLE)
    msg.rtm.rtm_flags |= RTF_REJECT;
#endif
#ifdef RTF_BLACKHOLE
  if(a->dest == RTD_BLACKHOLE)
    msg.rtm.rtm_flags |= RTF_BLACKHOLE;
#endif

  /* This is really very nasty, but I'm not able
   * to add "(reject|blackhole)" route without
   * gateway set
   */
  if(!i)
  {
    i = HEAD(iface_list);

    WALK_LIST(j, iface_list)
    {
      if (j->flags & IF_LOOPBACK)
      {
        i = j;
        break;
      }
    }
  }

  gw = a->gw;

#ifdef IPV6
  /* Embed interface ID to link-local address */
  if (ipa_has_link_scope(gw))
    _I0(gw) = 0xfe800000 | (i->index & 0x0000ffff);
#endif

  sockaddr_fill(&dst,  BIRD_AF, net->n.prefix, NULL, 0);
  sockaddr_fill(&mask, BIRD_AF, ipa_mkmask(net->n.pxlen), NULL, 0);
  sockaddr_fill(&gate, BIRD_AF, gw, NULL, 0);

  switch (a->dest)
  {
    case RTD_ROUTER:
      msg.rtm.rtm_flags |= RTF_GATEWAY;
      msg.rtm.rtm_addrs |= RTA_GATEWAY;
      break;

#ifdef RTF_REJECT
    case RTD_UNREACHABLE:
#endif
#ifdef RTF_BLACKHOLE
    case RTD_BLACKHOLE:
#endif
    case RTD_DEVICE:
      if(i)
      {
#ifdef RTF_CLONING
        if (cmd == RTM_ADD && (i->flags & IF_MULTIACCESS) != IF_MULTIACCESS)	/* PTP */
          msg.rtm.rtm_flags |= RTF_CLONING;
#endif

        if(!i->addr) {
          log(L_ERR "KRT: interface %s has no IP addess", i->name);
          return -1;
        }

	sockaddr_fill(&gate, BIRD_AF, i->addr->ip, NULL, 0);
        msg.rtm.rtm_addrs |= RTA_GATEWAY;
      }
      break;
    default:
      bug("krt-sock: unknown flags, but not filtered");
  }

  msg.rtm.rtm_index = i->index;

  NEXTADDR(RTA_DST, dst);
  NEXTADDR(RTA_GATEWAY, gate);
  NEXTADDR(RTA_NETMASK, mask);

  l = body - (char *)&msg;
  msg.rtm.rtm_msglen = l;

  if ((l = write(p->sys.sk->fd, (char *)&msg, l)) < 0) {
    log(L_ERR "KRT: Error sending route %I/%d to kernel: %m", net->n.prefix, net->n.pxlen);
    return -1;
  }

  return 0;
}
