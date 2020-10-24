static int
iface_plists_equal(struct iface_patt *pa, struct iface_patt *pb)
{
  struct iface_patt_node *x, *y;

  x = HEAD(pa->ipn_list);
  y = HEAD(pb->ipn_list);
  while (x->n.next && y->n.next)
    {
      if ((x->positive != y->positive) ||
	  (!x->pattern && y->pattern) ||	/* This nasty lines where written by me... :-( Feela */
	  (!y->pattern && x->pattern) ||
	  ((x->pattern != y->pattern) && strcmp(x->pattern, y->pattern)) ||
	  !ipa_equal(x->prefix, y->prefix) ||
	  (x->pxlen != y->pxlen))
	return 0;
      x = (void *) x->n.next;
      y = (void *) y->n.next;
    }
  return (!x->n.next && !y->n.next);
}
