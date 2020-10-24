radv_prepare_ra(struct radv_iface *ifa)
{
  struct proto_radv *ra = ifa->ra;
  struct radv_config *cf = (struct radv_config *) (ra->p.cf);
  struct radv_iface_config *ic = ifa->cf;

  char *buf = ifa->sk->tbuf;
  char *bufstart = buf;
  char *bufend = buf + ifa->sk->tbsize;

  struct radv_ra_packet *pkt = (void *) buf;
  pkt->type = ICMPV6_RA;
  pkt->code = 0;
  pkt->checksum = 0;
  pkt->current_hop_limit = ic->current_hop_limit;
  pkt->router_lifetime = (ra->active || !ic->default_lifetime_sensitive) ?
    htons(ic->default_lifetime) : 0;
  pkt->flags = (ic->managed ? OPT_RA_MANAGED : 0) |
    (ic->other_config ? OPT_RA_OTHER_CFG : 0) |
    (pkt->router_lifetime ? ic->default_preference : 0);
  pkt->reachable_time = htonl(ic->reachable_time);
  pkt->retrans_timer = htonl(ic->retrans_timer);
  buf += sizeof(*pkt);

  if (ic->link_mtu)
  {
    struct radv_opt_mtu *om = (void *) buf;
    om->type = OPT_MTU;
    om->length = 1;
    om->reserved = 0;
    om->mtu = htonl(ic->link_mtu);
    buf += sizeof (*om);
  }

  struct ifa *addr;
  WALK_LIST(addr, ifa->iface->addrs)
  {
    struct radv_prefix_config *pc;
    pc = radv_prefix_match(ifa, addr);

    if (!pc || pc->skip)
      continue;

    if (buf + sizeof(struct radv_opt_prefix) > bufend)
    {
      log(L_WARN "%s: Too many prefixes on interface %s", ra->p.name, ifa->iface->name);
      goto done;
    }

    struct radv_opt_prefix *op = (void *) buf;
    op->type = OPT_PREFIX;
    op->length = 4;
    op->pxlen = addr->pxlen;
    op->flags = (pc->onlink ? OPT_PX_ONLINK : 0) |
      (pc->autonomous ? OPT_PX_AUTONOMOUS : 0);
    op->valid_lifetime = (ra->active || !pc->valid_lifetime_sensitive) ?
      htonl(pc->valid_lifetime) : 0;
    op->preferred_lifetime = (ra->active || !pc->preferred_lifetime_sensitive) ?
      htonl(pc->preferred_lifetime) : 0;
    op->reserved = 0;
    op->prefix = addr->prefix;
    ipa_hton(op->prefix);
    buf += sizeof(*op);
  }

  if (! ic->rdnss_local)
    if (radv_prepare_rdnss(ifa, &cf->rdnss_list, &buf, bufend) < 0)
      goto done;

  if (radv_prepare_rdnss(ifa, &ic->rdnss_list, &buf, bufend) < 0)
    goto done;

  if (! ic->dnssl_local)
    if (radv_prepare_dnssl(ifa, &cf->dnssl_list, &buf, bufend) < 0)
      goto done;

  if (radv_prepare_dnssl(ifa, &ic->dnssl_list, &buf, bufend) < 0)
    goto done;

 done:
  ifa->plen = buf - bufstart;
}
