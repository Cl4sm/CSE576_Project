

/* Take a plotter, and open a file with a name that is either the
   title of the plot or "xplot.PS" with a number appended.  */

FILE *
make_name_open_file(struct plotter *pl)
{
  command *c;
  char *name = NULL, *versionp;
  static int version = 0;
  FILE *fp;

  for (c = pl->commands; c != NULL; c = c->next)  {
    if (c->type == TITLE && name == NULL)  {
      /* Allow space for the number. */
      name = malloc((unsigned)strlen(c->text) + 15);
      if (name == 0) fatalerror("malloc returned null");
#if 0
      (void) strcpy(name, c->text);
#else
      {
	char *from = c->text;
	char *to = name;
	char c;
	
	while (*from != '\0') {
	  switch (c = *from++) {
	  case '/':
	    *to++ = '_';
	    break;
	  default:
	    *to++ = c;
	  }
	}
	*to = '\0';
      }
#endif

    }
  }

  /* If no title found, just call it "xplot" */
  if (!name)  {
    name = malloc(sizeof("xplot") + 15);
    if (name == 0) fatalerror("malloc returned null");
    (void) strcpy(name, "xplot");
  }

  (void) strcat(name, ".PS.");
  versionp = name + strlen(name);
  do  {
    (void) sprintf(versionp, "%d", version++);
  } while (access(name, F_OK) == 0);