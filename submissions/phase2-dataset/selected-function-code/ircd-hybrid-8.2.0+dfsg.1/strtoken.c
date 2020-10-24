strtoken(char** save, char* str, const char* fs)
{
  char *pos = *save;  /* keep last position across calls */
  char *tmp = NULL;

  if (str)
    pos = str;    /* new string scan */

  while (pos && *pos && strchr(fs, *pos))
    ++pos;        /* skip leading separators */

  if (!pos || !*pos)
    return (pos = *save = NULL);   /* string contains only sep's */

  tmp = pos;       /* now, keep position of the token */

  while (*pos && strchr(fs, *pos) == NULL)
    ++pos;       /* skip content of the token */

  if (*pos)
    *pos++ = '\0';    /* remove first sep after the token */
  else
    pos = NULL;    /* end of string */

  *save = pos;
  return tmp;
}
