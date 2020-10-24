read_config (const char *config)
{
  FILE *file = fopen (config, "r");
  char *line = NULL;
  size_t len, llen;
  int ret = 0;
  struct config_line *c;

  if (file == NULL)
    {
      error (0, errno, "Can't open configuration file %s", config);
      return 1;
    }

  do
    {
      ssize_t i = getline (&line, &len, file);
      char *p;

      if (i < 0)
	break;

      if (line[i - 1] == '\n')
	line[i - 1] = '\0';

      p = strchr (line, '#');
      if (p != NULL)
	*p = '\0';

      p = line + strspn (line, " \t");
      if (p[0] == '-' && p[1] == 'c' && (p[2] == ' ' || p[2] == '\t'))
	{
	  glob_t g;
	  p += 2 + strspn (p + 2, " \t");

	  if (!glob (p, GLOB_BRACE, NULL, &g))
	    {
	      size_t n;

	      for (n = 0; n < g.gl_pathc; ++n)
		if (read_config (g.gl_pathv[n]))
		  {
		    ret = 1;
		    break;
		  }

	      globfree (&g);
	      if (ret)
		break;
	    }
	  continue;
	}

      llen = strlen (p);
      c = malloc (sizeof (*c) + llen);
      if (c == NULL)
	{
	  error (0, ENOMEM, "Could not cache config file");
	  ret = 1;
	  break;
	}

      c->next = NULL;
      memcpy (c->line, p, llen + 1);
      *config_end = c;
      config_end = &c->next;
    }
  while (!feof (file));

  free (line);
  fclose (file);
  return ret;
}
