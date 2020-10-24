static void *
birdloop_main(void *arg)
{
  struct birdloop *loop = arg;
  timer2 *t;
  int rv, timeout;

  birdloop_set_current(loop);

  pthread_mutex_lock(&loop->mutex);
  while (1)
  {
    events_fire(loop);
    timers_fire(loop);

    times_update(loop);
    if (events_waiting(loop))
      timeout = 0;
    else if (t = timers_first(loop))
      timeout = (tm2_remains(t) TO_MS) + 1;
    else
      timeout = -1;

    if (loop->poll_changed)
      sockets_prepare(loop);

    loop->poll_active = 1;
    pthread_mutex_unlock(&loop->mutex);

  try:
    rv = poll(loop->poll_fd.data, loop->poll_fd.used, timeout);
    if (rv < 0)
    {
      if (errno == EINTR || errno == EAGAIN)
	goto try;
      die("poll: %m");
    }

    pthread_mutex_lock(&loop->mutex);
    loop->poll_active = 0;

    if (loop->close_scheduled)
      sockets_close_fds(loop);

    if (loop->stop_called)
      break;

    if (rv)
      sockets_fire(loop);

    timers_fire(loop);
  }

  loop->stop_called = 0;
  pthread_mutex_unlock(&loop->mutex);

  return NULL;
}
