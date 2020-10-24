log_commit(int class, buffer *buf)
{
  struct log_config *l;

  if (buf->pos == buf->end)
    strcpy(buf->end - 100, " ... <too long>");

  log_lock();
  WALK_LIST(l, *current_log_list)
    {
      if (!(l->mask & (1 << class)))
	continue;
      if (l->fh)
	{
	  if (l->terminal_flag)
	    fputs("bird: ", l->fh);
	  else
	    {
	      byte tbuf[TM_DATETIME_BUFFER_SIZE];
	      tm_format_datetime(tbuf, &config->tf_log, now);
	      fprintf(l->fh, "%s <%s> ", tbuf, class_names[class]);
	    }
	  fputs(buf->start, l->fh);
	  fputc('\n', l->fh);
	  fflush(l->fh);
	}
#ifdef HAVE_SYSLOG
      else
	syslog(syslog_priorities[class], "%s", buf->start);
#endif
    }
  log_unlock();

  /* cli_echo is not thread-safe, so call it just from the main thread */
  if (main_thread_self())
    cli_echo(class, buf->start);

  buf->pos = buf->start;
}
