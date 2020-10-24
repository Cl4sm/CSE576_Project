static inline void
write_pid_file(void)
{
  int pl, rv;
  char ps[24];

  if (!pid_file)
    return;

  /* We don't use PID file for uniqueness, so no need for locking */

  pl = bsnprintf(ps, sizeof(ps), "%ld\n", (long) getpid());
  if (pl < 0)
    bug("PID buffer too small");

  rv = ftruncate(pid_fd, 0);
  if (rv < 0)
    die("fruncate: %m");
    
  rv = write(pid_fd, ps, pl);
  if(rv < 0)
    die("write: %m");

  close(pid_fd);
}
