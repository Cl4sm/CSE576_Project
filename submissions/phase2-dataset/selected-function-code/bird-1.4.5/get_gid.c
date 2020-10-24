static inline gid_t
get_gid(const char *s)
{
  struct group *gr;
  char *endptr;
  long int rv;

  if (!s)
    return 0;
  
  errno = 0;
  rv = strtol(s, &endptr, 10);

  if (!errno && !*endptr)
    return rv;

  gr = getgrnam(s);
  if (!gr)
    die("Cannot find group '%s'", s);

  return gr->gr_gid;
}
