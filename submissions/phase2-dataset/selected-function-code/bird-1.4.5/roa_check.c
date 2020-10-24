roa_check(struct roa_table *t, ip_addr prefix, byte pxlen, u32 asn)
{
  struct roa_node *n;
  ip_addr px;
  byte anything = 0;

  int len;
  for (len = pxlen; len >= 0; len--)
    {
      px = ipa_and(prefix, ipa_mkmask(len));
      n = fib_find(&t->fib, &px, len);

      if (!n)
	continue;

      struct roa_item *it;
      for (it = n->items; it; it = it->next)
	{
	  anything = 1;
	  if ((it->maxlen >= pxlen) && (it->asn == asn) && asn)
	    return ROA_VALID;
	}
    }

  return anything ? ROA_INVALID : ROA_UNKNOWN;
}
