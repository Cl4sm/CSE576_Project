int
evloop_remove_timer(int id)
{
  int found;
  int ret;

  struct pommed_timer *t;
  struct pommed_timer *pt;
  struct pommed_timer_job *j;
  struct pommed_timer_job *pj;

  found = 0;
  for (pt = NULL, t = timers; t != NULL; pt = t, t = t->next)
    {
      for (pj = NULL, j = t->jobs; j != NULL; pj = j, j = j->next)
	{
	  if (j->id == id)
	    {
	      if (pj != NULL)
		pj->next = j->next;
	      else
		t->jobs = j->next;

	      free(j);

	      found = 1;

	      break;
	    }
	}

      if (found)
	break;
    }

  if (t == NULL)
    return 0;

  if (t->jobs == NULL)
    {
      ret = evloop_remove(t->fd);
      if (ret < 0)
	return ret;

      close(t->fd);

      if (pt != NULL)
	pt->next = t->next;
      else
	timers = t->next;

      free(t);
    }

  return 0;
}