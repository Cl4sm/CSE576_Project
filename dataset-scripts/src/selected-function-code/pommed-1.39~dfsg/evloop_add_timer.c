int
evloop_add_timer(int timeout, pommed_timer_cb cb)
{
  int fd;

  struct pommed_timer *t;
  struct pommed_timer_job *j;

  j = (struct pommed_timer_job *)malloc(sizeof(struct pommed_timer_job));
  if (j == NULL)
    {
      logmsg(LOG_ERR, "Could not allocate memory for timer job");
      return -1;
    }

  j->cb = cb;
  j->id = timer_job_id;
  timer_job_id++;

  for (t = timers; t != NULL; t = t->next)
    {
      if (t->timeout == timeout)
	break;
    }

  if (t == NULL)
    {
      t = (struct pommed_timer *)malloc(sizeof(struct pommed_timer));
      if (t == NULL)
	{
	  logmsg(LOG_ERR, "Could not allocate memory for timer");
	  return -1;
	}

      fd = evloop_create_timer(timeout);
      if (fd < 0)
	{
	  free(t);
	  return -1;
	}

      t->fd = fd;
      t->timeout = timeout;
      t->jobs = NULL;
      t->next = timers;
      timers = t;
    }

  j->next = t->jobs;
  t->jobs = j;

  return 0;
}