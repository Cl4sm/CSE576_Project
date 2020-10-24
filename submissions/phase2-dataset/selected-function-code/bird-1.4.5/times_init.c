times_init(struct birdloop *loop)
{
  struct timespec ts;
  int rv;

  rv = clock_gettime(CLOCK_MONOTONIC, &ts);
  if (rv < 0)
  {
    log(L_WARN "Monotonic clock is missing");

    loop->use_monotonic_clock = 0;
    loop->last_time = 0;
    loop->real_time = 0;
    times_update_alt(loop);
    return;
  }

  if ((ts.tv_sec < 0) || (((s64) ts.tv_sec) > ((s64) 1 << 40)))
    log(L_WARN "Monotonic clock is crazy");

  loop->use_monotonic_clock = 1;
  loop->last_time = ((s64) ts.tv_sec S) + (ts.tv_nsec / 1000);
  loop->real_time = 0;
}
