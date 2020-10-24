rt_prune_loop(void)
{
  static int step = 0;
  int limit = 512;
  rtable *t;

 again:
  WALK_LIST(t, routing_tables)
    if (! rt_prune_step(t, step, &limit))
      return 0;

  if (step == 0)
    {
      /* Prepare for the second step */
      WALK_LIST(t, routing_tables)
	t->prune_state = RPS_SCHEDULED;

      step = 1;
      goto again;
    }

  /* Done */
  step = 0;
  return 1;
}
