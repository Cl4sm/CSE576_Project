void
krt_got_route(struct krt_proto *p, rte *e)
{
  rte *old;
  net *net = e->net;
  int verdict;

#ifdef KRT_ALLOW_LEARN
  switch (e->u.krt.src)
    {
    case KRT_SRC_KERNEL:
      verdict = KRF_IGNORE;
      goto sentenced;

    case KRT_SRC_REDIRECT:
      verdict = KRF_DELETE;
      goto sentenced;

    case  KRT_SRC_ALIEN:
      if (KRT_CF->learn)
	krt_learn_scan(p, e);
      else
	{
	  krt_trace_in_rl(&rl_alien, p, e, "[alien] ignored");
	  rte_free(e);
	}
      return;
    }
#endif
  /* The rest is for KRT_SRC_BIRD (or KRT_SRC_UNKNOWN) */

  if (net->n.flags & KRF_VERDICT_MASK)
    {
      /* Route to this destination was already seen. Strange, but it happens... */
      krt_trace_in(p, e, "already seen");
      rte_free(e);
      return;
    }

  if (!p->ready)
    {
      /* We wait for the initial feed to have correct KRF_INSTALLED flag */
      verdict = KRF_IGNORE;
      goto sentenced;
    }

  old = net->routes;
  if ((net->n.flags & KRF_INSTALLED) && rte_is_valid(old))
    {
      /* There may be changes in route attributes, we ignore that.
         Also, this does not work well if gw is changed in export filter */
      if ((net->n.flags & KRF_SYNC_ERROR) || ! krt_same_dest(e, old))
	verdict = KRF_UPDATE;
      else
	verdict = KRF_SEEN;
    }
  else
    verdict = KRF_DELETE;

 sentenced:
  krt_trace_in(p, e, ((char *[]) { "?", "seen", "will be updated", "will be removed", "ignored" }) [verdict]);
  net->n.flags = (net->n.flags & ~KRF_VERDICT_MASK) | verdict;
  if (verdict == KRF_UPDATE || verdict == KRF_DELETE)
    {
      /* Get a cached copy of attributes and temporarily link the route */
      rta *a = e->attrs;
      a->source = RTS_DUMMY;
      e->attrs = rta_lookup(a);
      e->next = net->routes;
      net->routes = e;
    }
  else
    rte_free(e);
}
