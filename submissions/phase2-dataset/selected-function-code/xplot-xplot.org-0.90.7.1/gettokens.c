  XCloseDisplay(dpy);
  return 0;
}

#ifdef __GNUC__
inline
#endif
static char **gettokens(FILE *fp)
{
  static char buf[1000];
  static char *tokens[1000];
  char *cp;
  int i;

  if (fgets(buf, sizeof(buf), fp) == NULL)
    return 0;

  i=0;
  cp=buf; 

  while (*cp != '\0') {
    while (*cp == ' ' || *cp == '\t') cp++;
    if (*cp == '\n') break;
    tokens[i++] = cp;
    while (*cp != ' ' && *cp != '\t' && *cp != '\n' && *cp != '\0') cp++;
    if (*cp == '\n' || *cp == '\0') {
      break;
    }
    *cp = '\0';
    cp++;
  }