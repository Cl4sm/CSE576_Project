tm_shot(void)
{
  timer *t;
  node *n, *m;

  if (first_far_timer <= now)
    {
      bird_clock_t limit = now + NEAR_TIMER_LIMIT;
      first_far_timer = TIME_INFINITY;
      n = HEAD(far_timers);
      while (m = n->next)
	{
	  t = SKIP_BACK(timer, n, n);
	  if (t->expires <= limit)
	    {
	      rem_node(n);
	      tm_insert_near(t);
	    }
	  else if (t->expires < first_far_timer)
	    first_far_timer = t->expires;
	  n = m;
	}
    }
  while ((n = HEAD(near_timers)) -> next)
    {
      int delay;
      t = SKIP_BACK(timer, n, n);
      if (t->expires > now)
	break;
      rem_node(n);
      delay = t->expires - now;
      t->expires = 0;
      if (t->recurrent)
	{
	  int i = t->recurrent - delay;
	  if (i < 0)
	    i = 0;
	  tm_start(t, i);
	}
      t->hook(t);
    }
}
