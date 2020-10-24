static inline int
rte_validate(rte *e)
{
  int c;
  net *n = e->net;

  if ((n->n.pxlen > BITS_PER_IP_ADDRESS) || !ip_is_prefix(n->n.prefix,n->n.pxlen))
    {
      log(L_WARN "Ignoring bogus prefix %I/%d received via %s",
	  n->n.prefix, n->n.pxlen, e->sender->proto->name);
      return 0;
    }

  c = ipa_classify_net(n->n.prefix);
  if ((c < 0) || !(c & IADDR_HOST) || ((c & IADDR_SCOPE_MASK) <= SCOPE_LINK))
    {
      log(L_WARN "Ignoring bogus route %I/%d received via %s",
	  n->n.prefix, n->n.pxlen, e->sender->proto->name);
      return 0;
    }

  return 1;
}
