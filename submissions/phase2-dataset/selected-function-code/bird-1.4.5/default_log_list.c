default_log_list(int debug, int init, char **syslog_name)
{
  static list init_log_list;
  init_list(&init_log_list);
  *syslog_name = NULL;

#ifdef HAVE_SYSLOG
  if (!debug)
    {
      static struct log_config lc_syslog = { mask: ~0 };
      add_tail(&init_log_list, &lc_syslog.n);
      *syslog_name = bird_name;
      if (!init)
	return &init_log_list;
    }
#endif

  static struct log_config lc_stderr = { mask: ~0, terminal_flag: 1 };
  lc_stderr.fh = stderr;
  add_tail(&init_log_list, &lc_stderr.n);
  return &init_log_list;
}
