ilog(enum log_type type, const char *fmt, ...)
{
  char buf[LOG_BUFSIZE] = "";
  va_list args;

  if (!log_type_table[type].file || !ConfigLog.use_logging)
    return;

  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  log_write(type, buf);

  if (log_exceed_size(type) <= 0)
    return;

  snprintf(buf, sizeof(buf), "Rotating logfile %s",
           log_type_table[type].path);
  log_write(type, buf);

  fclose(log_type_table[type].file);
  log_type_table[type].file = NULL;

  snprintf(buf, sizeof(buf), "%s.old", log_type_table[type].path);
  unlink(buf);
  rename(log_type_table[type].path, buf);

  log_set_file(type, log_type_table[type].size, log_type_table[type].path);
  log_type_table[type].file = fopen(log_type_table[type].path, "a");
}
