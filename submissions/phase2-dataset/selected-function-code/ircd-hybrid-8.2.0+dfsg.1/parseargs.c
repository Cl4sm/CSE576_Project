parseargs(int *argc, char ***argv, struct lgetopt *opts)
{
  const char *progname = (*argv)[0];

  /* Loop through each argument */
  while (1)
  {
    int found = 0;
    const char *opt = NULL;

    (*argc)--;
    (*argv)++;

    if (*argc < 1 || (*argv)[0][0] != OPTCHAR)
      return;

    opt = &(*argv)[0][1];

    /* Search through our argument list, and see if it matches */
    for (unsigned int i = 0; opts[i].opt; ++i)
    {
      if (!strcmp(opts[i].opt, opt))
      {
        /* Found our argument */
        found = 1;

        switch (opts[i].argtype)
        {
          case YESNO:
            *((int *)opts[i].argloc) = 1;
            break;

          case INTEGER:
            if (*argc < 2)
            {
              fprintf(stderr, "Error: option '%c%s' requires an argument\n",
                      OPTCHAR, opts[i].opt);
              usage((*argv)[0], opts);
            }

            *((int *)opts[i].argloc) = atoi((*argv)[1]);
            (*argc)--;
            (*argv)++;
            break;

          case STRING:
            if (*argc < 2)
            {
              fprintf(stderr, "error: option '%c%s' requires an argument\n",
                      OPTCHAR, opts[i].opt);
              usage(progname, opts);
            }

            *((char **)opts[i].argloc) = xstrdup((*argv)[1]);
            (*argc)--;
            (*argv)++;
            break;

          case USAGE:
            usage(progname, opts);
            /* NOTREACHED */

          default:
            fprintf(stderr, "Error: internal error in parseargs() at %s:%d\n",
                    __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }
      }
    }

    if (!found)
    {
      fprintf(stderr, "error: unknown argument '%c%s'\n",
              OPTCHAR, opt);
      usage(progname, opts);
    }
  }
}
