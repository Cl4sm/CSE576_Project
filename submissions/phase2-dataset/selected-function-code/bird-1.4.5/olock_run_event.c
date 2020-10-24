static void
olock_run_event(void *unused UNUSED)
{
  node *n;
  struct object_lock *q;

  DBG("olock: Processing events\n");
  for(;;)
    {
      n = HEAD(olock_list);
      if (!n->next)
	break;
      q = SKIP_BACK(struct object_lock, n, n);
      if (q->state != OLOCK_STATE_EVENT)
	break;
      DBG("olock: %p locked\n", q);
      q->state = OLOCK_STATE_LOCKED;
      rem_node(&q->n);
      add_tail(&olock_list, &q->n);
      q->hook(q);
    }
}
