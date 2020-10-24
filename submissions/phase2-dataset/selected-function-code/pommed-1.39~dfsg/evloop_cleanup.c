void
evloop_cleanup(void)
{
  struct pommed_event *p;
  struct pommed_timer *t;
  struct pommed_timer_job *j;
  struct pommed_timer_job *jobs;

  close(epfd);

  while (sources != NULL)
    {
      p = sources;
      sources = sources->next;

      close(p->fd);

      free(p);
    }

  while (timers != NULL)
    {
      t = timers;
      timers = timers->next;

      jobs = t->jobs;
      while (jobs != NULL)
	{
	  j = jobs;
	  jobs = jobs->next;

	  free(j);
	}

      free(t);
    }
}