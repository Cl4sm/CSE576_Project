birdloop_new(void)
{
  /* FIXME: this init should be elsewhere and thread-safe */
  static int init = 0;
  if (!init)
    { birdloop_init_current(); init = 1; }

  pool *p = rp_new(NULL, "Birdloop root");
  struct birdloop *loop = mb_allocz(p, sizeof(struct birdloop));
  loop->pool = p;
  pthread_mutex_init(&loop->mutex, NULL);

  times_init(loop);
  wakeup_init(loop);

  events_init(loop);
  timers_init(loop);
  sockets_init(loop);

  return loop;
}
