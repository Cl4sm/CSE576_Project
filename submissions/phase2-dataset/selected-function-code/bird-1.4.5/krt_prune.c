krt_prune(struct krt_proto *p)
{
  struct rtable *t = p->p.table;

  KRT_TRACE(p, D_EVENTS, "Pruning table %s", t->name);
  FIB_WALK(&t->fib, f)
    {
      net *n = (net *) f;
      int verdict = f->flags & KRF_VERDICT_MASK;
      rte *new, *new0, *old;
      ea_list *tmpa = NULL;

      if (verdict == KRF_UPDATE || verdict == KRF_DELETE)
	{
	  /* Get a dummy route from krt_got_route() */
	  old = n->routes;
	  n->routes = old->next;
	}
      else
	old = NULL;

      new = new0 = n->routes;
      if (verdict == KRF_CREATE || verdict == KRF_UPDATE)
	{
	  /* We have to run export filter to get proper 'new' route */
	  if (! krt_export_rte(p, &new, &tmpa))
	    {
	      /* Route rejected, should not happen (KRF_INSTALLED) but to be sure .. */
	      verdict = (verdict == KRF_CREATE) ? KRF_IGNORE : KRF_DELETE;
	    }
	  else
	    {
	      ea_list **x = &tmpa;
	      while (*x)
		x = &((*x)->next);
	      *x = new ? new->attrs->eattrs : NULL;
	    }
	}

      switch (verdict)
	{
	case KRF_CREATE:
	  if (new && (f->flags & KRF_INSTALLED))
	    {
	      krt_trace_in(p, new, "reinstalling");
	      krt_replace_rte(p, n, new, NULL, tmpa);
	    }
	  break;
	case KRF_SEEN:
	case KRF_IGNORE:
	  /* Nothing happens */
	  break;
	case KRF_UPDATE:
	  krt_trace_in(p, new, "updating");
	  krt_replace_rte(p, n, new, old, tmpa);
	  break;
	case KRF_DELETE:
	  krt_trace_in(p, old, "deleting");
	  krt_replace_rte(p, n, NULL, old, NULL);
	  break;
	default:
	  bug("krt_prune: invalid route status");
	}

      if (old)
	rte_free(old);
      if (new != new0)
	rte_free(new);
      lp_flush(krt_filter_lp);
      f->flags &= ~KRF_VERDICT_MASK;
    }
  FIB_WALK_END;

#ifdef KRT_ALLOW_LEARN
  if (KRT_CF->learn)
    krt_learn_prune(p);
#endif

  if (p->ready)
    p->initialized = 1;
}
