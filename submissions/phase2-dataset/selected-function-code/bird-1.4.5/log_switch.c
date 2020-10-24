void
log_switch(int debug, list *l, char *new_syslog_name)
{
  if (!l || EMPTY_LIST(*l))
    l = default_log_list(debug, !l, &new_syslog_name);

  current_log_list = l;

#ifdef HAVE_SYSLOG
  if (current_syslog_name && new_syslog_name &&
      !strcmp(current_syslog_name, new_syslog_name))
    return;

  if (current_syslog_name)
    closelog();

  if (new_syslog_name)
    openlog(new_syslog_name, LOG_CONS | LOG_NDELAY, LOG_DAEMON);

  current_syslog_name = new_syslog_name;
#endif
}
