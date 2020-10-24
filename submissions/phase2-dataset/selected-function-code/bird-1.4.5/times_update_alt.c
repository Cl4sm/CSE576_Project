static void
times_update_alt(struct birdloop *loop)
{
  struct timeval tv;
  int rv;

  rv = gettimeofday(&tv, NULL);
  if (rv < 0)
    die("gettimeofday: %m");

  btime new_time = ((s64) tv.tv_sec S) + tv.tv_usec;
  btime delta = new_time - loop->real_time;

  if ((delta < 0) || (delta > (60 S)))
  {
    if (loop->real_time)
      log(L_WARN "Time jump, delta %d us", (int) delta);

    delta = 100 MS;
  }

  loop->last_time += delta;
  loop->real_time = new_time;
}
