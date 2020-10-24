void
io_loop(void)
{
  fd_set rd, wr;
  struct timeval timo;
  time_t tout;
  int hi, events;
  sock *s;
  node *n;

  sock_recalc_fdsets_p = 1;
  for(;;)
    {
      events = ev_run_list(&global_event_list);
      update_times();
      tout = tm_first_shot();
      if (tout <= now)
	{
	  tm_shot();
	  continue;
	}
      timo.tv_sec = events ? 0 : MIN(tout - now, 3);
      timo.tv_usec = 0;

      if (sock_recalc_fdsets_p)
	{
	  sock_recalc_fdsets_p = 0;
	  FD_ZERO(&rd);
	  FD_ZERO(&wr);
	}

      hi = 0;
      WALK_LIST(n, sock_list)
	{
	  s = SKIP_BACK(sock, n, n);
	  if (s->rx_hook)
	    {
	      FD_SET(s->fd, &rd);
	      if (s->fd > hi)
		hi = s->fd;
	    }
	  else
	    FD_CLR(s->fd, &rd);
	  if (s->tx_hook && s->ttx != s->tpos)
	    {
	      FD_SET(s->fd, &wr);
	      if (s->fd > hi)
		hi = s->fd;
	    }
	  else
	    FD_CLR(s->fd, &wr);
	}

      /*
       * Yes, this is racy. But even if the signal comes before this test
       * and entering select(), it gets caught on the next timer tick.
       */

      if (async_config_flag)
	{
	  async_config();
	  async_config_flag = 0;
	  continue;
	}
      if (async_dump_flag)
	{
	  async_dump();
	  async_dump_flag = 0;
	  continue;
	}
      if (async_shutdown_flag)
	{
	  async_shutdown();
	  async_shutdown_flag = 0;
	  continue;
	}

      /* And finally enter select() to find active sockets */
      hi = select(hi+1, &rd, &wr, NULL, &timo);

      if (hi < 0)
	{
	  if (errno == EINTR || errno == EAGAIN)
	    continue;
	  die("select: %m");
	}
      if (hi)
	{
	  /* guaranteed to be non-empty */
	  current_sock = SKIP_BACK(sock, n, HEAD(sock_list));

	  while (current_sock)
	    {
	      sock *s = current_sock;
	      int e;
	      int steps;

	      steps = MAX_STEPS;
	      if ((s->type >= SK_MAGIC) && FD_ISSET(s->fd, &rd) && s->rx_hook)
		do
		  {
		    steps--;
		    e = sk_read(s);
		    if (s != current_sock)
		      goto next;
		  }
		while (e && s->rx_hook && steps);

	      steps = MAX_STEPS;
	      if (FD_ISSET(s->fd, &wr))
		do
		  {
		    steps--;
		    e = sk_write(s);
		    if (s != current_sock)
		      goto next;
		  }
		while (e && steps);
	      current_sock = sk_next(s);
	    next: ;
	    }

	  short_loops++;
	  if (events && (short_loops < SHORT_LOOP_MAX))
	    continue;
	  short_loops = 0;

	  int count = 0;
	  current_sock = stored_sock;
	  if (current_sock == NULL)
	    current_sock = SKIP_BACK(sock, n, HEAD(sock_list));

	  while (current_sock && count < MAX_RX_STEPS)
	    {
	      sock *s = current_sock;
	      int e UNUSED;

	      if ((s->type < SK_MAGIC) && FD_ISSET(s->fd, &rd) && s->rx_hook)
		{
		  count++;
		  e = sk_read(s);
		  if (s != current_sock)
		      goto next2;
		}
	      current_sock = sk_next(s);
	    next2: ;
	    }

	  stored_sock = current_sock;
	}
    }
}
