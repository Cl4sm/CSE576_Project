void
logmsg(int level, char *fmt, ...)
{
  va_list ap;
  FILE *where = stdout;

  va_start(ap, fmt);

  if (console)
    {
      switch (level)
	{
	  case LOG_INFO:
	    fprintf(where, "I: ");
	    break;

	  case LOG_WARNING:
	    fprintf(where, "W: ");
	    break;

	  case LOG_ERR:
	    where = stderr;
	    fprintf(where, "E: ");
	    break;

	  default:
	    break;
	}
      vfprintf(where, fmt, ap);
      fprintf(where, "\n");
    }
  else
    {
      vsyslog(level | LOG_DAEMON, fmt, ap);
    }

  va_end(ap);
}