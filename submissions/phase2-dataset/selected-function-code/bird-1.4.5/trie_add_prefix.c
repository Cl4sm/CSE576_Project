void
trie_add_prefix(struct f_trie *t, ip_addr px, int plen, int l, int h)
{
  if (l == 0)
    t->zero = 1;
  else
    l--;

  if (h < plen)
    plen = h;

  ip_addr amask = ipa_xor(ipa_mkmask(l), ipa_mkmask(h));
  ip_addr pmask = ipa_mkmask(plen);
  ip_addr paddr = ipa_and(px, pmask);
  struct f_trie_node *o = NULL;
  struct f_trie_node *n = &t->root;

  while(n)
    {
      ip_addr cmask = ipa_and(n->mask, pmask);

      if (ipa_compare(ipa_and(paddr, cmask), ipa_and(n->addr, cmask)))
	{
	  /* We are out of path - we have to add branching node 'b'
	     between node 'o' and node 'n', and attach new node 'a'
	     as the other child of 'b'. */
	  int blen = ipa_pxlen(paddr, n->addr);
	  ip_addr bmask = ipa_mkmask(blen);
	  ip_addr baddr = ipa_and(px, bmask);

	  /* Merge accept masks from children to get accept mask for node 'b' */
	  ip_addr baccm = ipa_and(ipa_or(amask, n->accept), bmask);

	  struct f_trie_node *a = new_node(t, plen, paddr, pmask, amask);
	  struct f_trie_node *b = new_node(t, blen, baddr, bmask, baccm);
	  attach_node(o, b);
	  attach_node(b, n);
	  attach_node(b, a);
	  return;
	}

      if (plen < n->plen)
	{
	  /* We add new node 'a' between node 'o' and node 'n' */
	  amask = ipa_or(amask, ipa_and(n->accept, pmask));
	  struct f_trie_node *a = new_node(t, plen, paddr, pmask, amask);
	  attach_node(o, a);
	  attach_node(a, n);
	  return;
	}
	
      if (plen == n->plen)
	{
	  /* We already found added node in trie. Just update accept mask */
	  n->accept = ipa_or(n->accept, amask);
	  return;
	}

      /* Update accept mask part M2 and go deeper */
      n->accept = ipa_or(n->accept, ipa_and(amask, n->mask));

      /* n->plen < plen and plen <= 32 (128) */
      o = n;
      n = n->c[ipa_getbit(paddr, n->plen) ? 1 : 0];
    }

  /* We add new tail node 'a' after node 'o' */
  struct f_trie_node *a = new_node(t, plen, paddr, pmask, amask);
  attach_node(o, a);
}
