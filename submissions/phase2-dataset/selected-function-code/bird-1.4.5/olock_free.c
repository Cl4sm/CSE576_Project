static void
olock_free(resource *r)
{
  struct object_lock *q, *l = (struct object_lock *) r;
  node *n;

  DBG("olock: Freeing %p\n", l);
  switch (l->state)
    {
    case OLOCK_STATE_FREE:
      break;
    case OLOCK_STATE_LOCKED:
    case OLOCK_STATE_EVENT:
      rem_node(&l->n);
      n = HEAD(l->waiters);
      if (n->next)
	{
	  DBG("olock: -> %p becomes locked\n", n);
	  q = SKIP_BACK(struct object_lock, n, n);
	  rem_node(n);
	  add_tail_list(&l->waiters, &q->waiters);
	  q->state = OLOCK_STATE_EVENT;
	  add_head(&olock_list, n);
	  ev_schedule(olock_event);
	}
      break;
    case OLOCK_STATE_WAITING:
      rem_node(&l->n);
      break;
    default:
      ASSERT(0);
    }
}
