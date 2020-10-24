static void
evloop_timer_callback(int fd, uint32_t events)
{
  uint64_t ticks;

  struct pommed_timer *t;
  struct pommed_timer_job *j;

  /* Acknowledge timer */
  read(fd, &ticks, sizeof(ticks));

  j = NULL;
  for (t = timers; t != NULL; t = t->next)
    {
      if (t->fd == fd)
	{
	  j = t->jobs;

	  break;
	}
    }

  while (j != NULL)
    {
      j->cb(j->id, ticks);

      j = j->next;
    }
}