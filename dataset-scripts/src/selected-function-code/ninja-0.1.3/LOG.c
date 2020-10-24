void LOG(char *fmt, ...)
{
  va_list args;
  FILE *fd;

  if (global_opts.quiet == FALSE) {
    va_start(args, fmt);
#ifdef LOG_PREFIX
    fprintf(stdout, "%s", LOG_PREFIX);
#endif
    vfprintf(stdout, fmt, args);
    puts("");
    va_end(args);
  }

  if (global_opts.logfile[0] != 0x00 && file_isregular(global_opts.logfile) == 1) {
    va_start(args, fmt);
    fd = fopen(global_opts.logfile, "a");
    if (fd != NULL) {
      fprintf(fd, "[%s ", replace(timestamp(), '\n', ']'));
      vfprintf(fd, fmt, args);
      fprintf(fd, "\n");
      fclose(fd);
    }
    va_end(args);
  }
  return;
}