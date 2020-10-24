rt_refresh_end(rtable *t, struct announce_hook *ah)
{
  int prune = 0;
  net *n;
  rte *e;

  FIB_WALK(&t->fib, fn)
    {
      n = (net *) fn;
      for (e = n->routes; e; e = e->next)
	if ((e->sender == ah) && (e->flags & REF_STALE))
	  {
	    e->flags |= REF_DISCARD;
	    prune = 1;
	  }
    }
  FIB_WALK_END;

  if (prune)
    rt_schedule_prune(t);
}
