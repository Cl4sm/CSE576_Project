krt_got_route_async(struct krt_proto *p, rte *e, int new)
{
  net *net = e->net;

  switch (e->u.krt.src)
    {
    case KRT_SRC_BIRD:
      ASSERT(0);			/* Should be filtered by the back end */

    case KRT_SRC_REDIRECT:
      if (new)
	{
	  krt_trace_in(p, e, "[redirect] deleting");
	  krt_replace_rte(p, net, NULL, e, NULL);
	}
      /* If !new, it is probably echo of our deletion */
      break;

#ifdef KRT_ALLOW_LEARN
    case KRT_SRC_ALIEN:
      if (KRT_CF->learn)
	{
	  krt_learn_async(p, e, new);
	  return;
	}
#endif
    }
  rte_free(e);
}
