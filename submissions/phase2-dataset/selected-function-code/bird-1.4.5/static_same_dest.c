static inline int
static_same_dest(struct static_route *x, struct static_route *y)
{
  if (x->dest != y->dest)
    return 0;

  switch (x->dest)
    {
    case RTD_ROUTER:
      return ipa_equal(x->via, y->via) && (x->via_if == y->via_if);

    case RTD_DEVICE:
      return !strcmp(x->if_name, y->if_name);

    case RTD_MULTIPATH:
      for (x = x->mp_next, y = y->mp_next;
	   x && y;
	   x = x->mp_next, y = y->mp_next)
	if (!ipa_equal(x->via, y->via) || (x->via_if != y->via_if))
	  return 0;
      return !x && !y;

    case RTDX_RECURSIVE:
      return ipa_equal(x->via, y->via);

    default:
      return 1;
    }
}
