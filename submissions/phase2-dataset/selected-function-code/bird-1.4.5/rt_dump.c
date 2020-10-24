rt_dump(rtable *t)
{
  rte *e;
  net *n;
  struct announce_hook *a;

  debug("Dump of routing table <%s>\n", t->name);
#ifdef DEBUGGING
  fib_check(&t->fib);
#endif
  FIB_WALK(&t->fib, fn)
    {
      n = (net *) fn;
      for(e=n->routes; e; e=e->next)
	rte_dump(e);
    }
  FIB_WALK_END;
  WALK_LIST(a, t->hooks)
    debug("\tAnnounces routes to protocol %s\n", a->proto->name);
  debug("\n");
}
