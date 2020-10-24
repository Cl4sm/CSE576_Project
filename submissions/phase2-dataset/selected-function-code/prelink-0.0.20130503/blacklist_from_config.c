blacklist_from_config (void)
{
  struct config_line *c;
  int ret = 0;

  implicit = 1;
  for (c = config_lines; c; c = c->next)
    {
      int deref = 0;
      int onefs = 0;
      int blacklist = 0;
      char *p = c->line;

      while (*p == '-')
	{
	  switch (p[1])
	    {
	    case 'h': deref = 1; break;
	    case 'l': onefs = 1; break;
	    case 'b': blacklist = 1; break;
	    }
	  p = p + 2 + strspn (p + 2, " \t");
	}

      if (*p == '\0' || !blacklist)
	continue;

      if (strchr (p, '/') == NULL)
	{
	  add_blacklist_ext (p);
	  continue;
	}

      if (strpbrk (p, "*?[{") == NULL)
	{
	  ret = add_to_blacklist (p, deref, onefs);
	  if (ret)
	    {
	      ret = 1;
	      break;
	    }
	}
      else
	{
	  glob_t g;

	  if (!glob (p, GLOB_BRACE | GLOB_PERIOD, NULL, &g))
	    {
	      size_t n;

	      for (n = 0; n < g.gl_pathc; ++n)
		{
		  ret = add_to_blacklist (g.gl_pathv[n], deref, onefs);
		  if (ret)
		    {
		      ret = 1;
		      break;
		    }
		}

	      globfree (&g);
	      if (ret)
		break;
	    }
	}
    }

  implicit = 0;
  return ret;
}
