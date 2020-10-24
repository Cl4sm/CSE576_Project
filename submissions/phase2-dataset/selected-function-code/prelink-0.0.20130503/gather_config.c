gather_config (void)
{
  struct config_line *c;
  int ret = 0;

  implicit = 1;
  for (c = config_lines; c; c = c->next)
    {
      int deref = 0;
      int onefs = 0;
      char *p = c->line;

      while (*p == '-')
	{
	  switch (p[1])
	    {
	    case 'h': deref = 1; break;
	    case 'l': onefs = 1; break;
	    case 'b': p = ""; continue;
	    default:
	      error (0, 0, "Unknown directory option `%s'\n", p);
	      break;
	    }
	  p = p + 2 + strspn (p + 2, " \t");
	}

      if (*p == '\0')
	continue;

      if (strpbrk (p, "*?[{") == NULL)
	{
	  ret = gather_object (p, deref, onefs);
	  if (ret)
	    {
	      ret = 1;
	      break;
	    }
	}
      else
	{
	  glob_t g;

	  if (!glob (p, GLOB_BRACE, NULL, &g))
	    {
	      size_t n;

	      for (n = 0; n < g.gl_pathc; ++n)
		{
		  ret = gather_object (g.gl_pathv[n], deref, onefs);
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
