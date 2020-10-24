int
trie_match_prefix(struct f_trie *t, ip_addr px, int plen)
{
  ip_addr pmask = ipa_mkmask(plen);
  ip_addr paddr = ipa_and(px, pmask);

  if (plen == 0)
    return t->zero;

  int plentest = plen - 1;
  struct f_trie_node *n = &t->root;

  while(n)
    {
      ip_addr cmask = ipa_and(n->mask, pmask);

      /* We are out of path */
      if (ipa_compare(ipa_and(paddr, cmask), ipa_and(n->addr, cmask)))
	return 0;

      /* Check accept mask */
      if (ipa_getbit(n->accept, plentest))
	return 1;

      /* We finished trie walk and still no match */
      if (plen <= n->plen)
	return 0;

      /* Choose children */
      n =  n->c[(ipa_getbit(paddr, n->plen)) ? 1 : 0];
    }

  return 0;
}
