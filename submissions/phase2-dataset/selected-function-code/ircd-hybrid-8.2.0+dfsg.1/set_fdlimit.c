set_fdlimit(void)
{
  int fdmax;
  struct rlimit limit;

  if (!getrlimit(RLIMIT_NOFILE, &limit))
  {
    limit.rlim_cur = limit.rlim_max;
    setrlimit(RLIMIT_NOFILE, &limit);
  }

  fdmax = getdtablesize();

  /* allow MAXCLIENTS_MIN clients even at the cost of MAX_BUFFER and
   * some not really LEAKED_FDS */
  fdmax = IRCD_MAX(fdmax, LEAKED_FDS + MAX_BUFFER + MAXCLIENTS_MIN);

  /* under no condition shall this raise over 65536
   * for example user ip heap is sized 2*hard_fdlimit */
  hard_fdlimit = IRCD_MIN(fdmax, 65536);

  return -1;
}
