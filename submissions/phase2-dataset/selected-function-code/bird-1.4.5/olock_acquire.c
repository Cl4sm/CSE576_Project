void
olock_acquire(struct object_lock *l)
{
  node *n;
  struct object_lock *q;

  WALK_LIST(n, olock_list)
    {
      q = SKIP_BACK(struct object_lock, n, n);
      if (olock_same(q, l))
	{
	  l->state = OLOCK_STATE_WAITING;
	  add_tail(&q->waiters, &l->n);
	  DBG("olock: %p waits\n", l);
	  return;
	}
    }
  DBG("olock: %p acquired immediately\n", l);
  l->state = OLOCK_STATE_EVENT;
  add_head(&olock_list, &l->n);
  ev_schedule(olock_event);
}
