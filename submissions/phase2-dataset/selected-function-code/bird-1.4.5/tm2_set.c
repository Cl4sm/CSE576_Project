tm2_set(timer2 *t, btime when)
{
  struct birdloop *loop = birdloop_current();
  uint tc = timers_count(loop);

  if (!t->expires)
  {
    t->index = ++tc;
    t->expires = when;
    BUFFER_PUSH(loop->timers) = t;
    HEAP_INSERT(loop->timers.data, tc, timer2 *, TIMER_LESS, TIMER_SWAP);
  }
  else if (t->expires < when)
  {
    t->expires = when;
    HEAP_INCREASE(loop->timers.data, tc, timer2 *, TIMER_LESS, TIMER_SWAP, t->index);
  }
  else if (t->expires > when)
  {
    t->expires = when;
    HEAP_DECREASE(loop->timers.data, tc, timer2 *, TIMER_LESS, TIMER_SWAP, t->index);
  }

  if (loop->poll_active && (t->index == 1))
    wakeup_kick(loop);
}
