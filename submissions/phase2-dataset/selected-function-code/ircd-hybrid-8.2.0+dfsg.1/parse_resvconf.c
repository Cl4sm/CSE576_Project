parse_resvconf(void)
{
  char *p;
  char *opt;
  char *arg;
  char input[MAXLINE];
  FILE *file;

  /* XXX "/etc/resolv.conf" should be from a define in config.h perhaps
   * for cygwin support etc. this hardcodes it to unix for now -db
   */
  if ((file = fopen("/etc/resolv.conf", "r")) == NULL)
    return;

  while (fgets(input, sizeof(input), file))
  {
    /* blow away any newline */
    if ((p = strpbrk(input, "\r\n")))
      *p = '\0';

    p = input;

    /* skip until something thats not a space is seen */
    while (IsSpace(*p))
      ++p;

    /* if at this point, have a '\0' then continue */
    if (*p == '\0')
      continue;

    /* Ignore comment lines immediately */
    if (*p == ';' || *p == '#')
      continue;

    /* skip until a space is found */
    opt = p;
    while (!IsSpace(*p) && *p)
      ++p;

    if (*p == '\0')
      continue;  /* no arguments?.. ignore this line */

    /* blow away the space character */
    *p++ = '\0';

    /* skip these spaces that are before the argument */
    while (IsSpace(*p))
      ++p;

    /* Now arg should be right where p is pointing */
    arg = p;

    if ((p = strpbrk(arg, " \t")))
      *p = '\0';  /* take the first word */

    if (!strcasecmp(opt, "nameserver"))
      add_nameserver(arg);
  }

  fclose(file);
}
