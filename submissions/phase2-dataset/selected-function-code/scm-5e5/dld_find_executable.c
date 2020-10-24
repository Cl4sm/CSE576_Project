     const char *name;
{
  char *search;
  register char *p;
  char tbuf[MAXPATHLEN];

  if (ABSOLUTE_FILENAME_P(name))
    return access(name, X_OK) ? 0 : copy_of(name);

  if (strchr(name, '/')) {
    strcpy (tbuf, ".");		/* in case getcwd doesn't work */
    getcwd(tbuf, MAXPATHLEN);
    if ((name[0] == '.') && (name[1] == '/')) {
      strcat(tbuf, name + 1);
    } else {
      if ('/' != tbuf[strlen(tbuf) - 1]) strcat(tbuf, "/");
      strcat(tbuf, name);
    }
    return copy_of(tbuf);
  }

  if (((search = (char *) getenv("DLDPATH")) == 0) &&
      ((search = (char *) getenv("PATH")) == 0))
    search = DEFAULT_PATH;

  p = search;

  while (*p) {
    register char *next = tbuf;

    if (p[0]=='~' && p[1]=='/' && getenv("HOME")) {
      strcpy(tbuf, (char *)getenv("HOME"));
      next = tbuf + strlen(tbuf);
      p++;
    }

    /* Copy directory name into [tbuf] */
    while (*p && *p != ':') *next++ = *p++;
    *next = 0;
    if (*p) p++;

    if (tbuf[0] == '.' && tbuf[1] == 0)
      getcwd(tbuf, MAXPATHLEN);	/* was getwd(tbuf); */
    else if (tbuf[0]=='~' && tbuf[1]==0 && getenv("HOME"))
      strcpy(tbuf, (char *)getenv("HOME"));

    strcat(tbuf, "/");
    strcat(tbuf, name);

    if (access(tbuf, X_OK) == 0) {
# ifndef hpux
#  ifndef ultrix
#   ifndef __MACH__
#    ifndef PLAN9
      struct stat stat_temp;
      if (stat(tbuf, &stat_temp)) continue;
      if (S_IFREG != (S_IFMT & stat_temp.st_mode)) continue;
#    endif /* PLAN9 */
#   endif /* __MACH__ */
#  endif/* ultrix */
# endif /* hpux */
      return copy_of(tbuf);
    }
  }

  return 0;
}
