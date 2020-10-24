void
tm_start(timer *t, unsigned after)
{
  bird_clock_t when;

  if (t->randomize)
    after += random() % (t->randomize + 1);
  when = now + after;
  if (t->expires == when)
    return;
  if (t->expires)
    rem_node(&t->n);
  t->expires = when;
  if (after <= NEAR_TIMER_LIMIT)
    tm_insert_near(t);
  else
    {
      if (!first_far_timer || first_far_timer > when)
	first_far_timer = when;
      add_tail(&far_timers, &t->n);
    }
}
