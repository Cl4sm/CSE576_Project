get_uid(const char *s)
{
  struct passwd *pw;
  char *endptr;
  long int rv;

  if (!s)
    return 0;

  errno = 0;
  rv = strtol(s, &endptr, 10);

  if (!errno && !*endptr)
    return rv;

  pw = getpwnam(s);
  if (!pw)
    die("Cannot find user '%s'", s);

  return pw->pw_uid;
}
