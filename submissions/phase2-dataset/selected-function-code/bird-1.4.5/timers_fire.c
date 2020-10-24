static void
timers_fire(struct birdloop *loop)
{
  btime base_time;
  timer2 *t;

  times_update(loop);
  base_time = loop->last_time;

  while (t = timers_first(loop))
  {
    if (t->expires > base_time)
      return;

    if (t->recurrent)
    {
      btime when = t->expires + t->recurrent;
      
      if (when <= loop->last_time)
	when = loop->last_time + t->recurrent;

      if (t->randomize)
	when += random() % (t->randomize + 1);

      tm2_set(t, when);
    }
    else
      tm2_stop(t);

    t->hook(t);
  }
}
