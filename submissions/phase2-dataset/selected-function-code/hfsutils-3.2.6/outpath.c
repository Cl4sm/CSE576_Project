int outpath(dstring *str, queueent *ent, int flags)
{
  const char *path;

  path = PATH(*ent);

  dstr_shrink(str, 0);

  if ((flags & HLS_QUOTE) &&
      dstr_append(str, "\"", 1) == -1)
    return -1;

  if (flags & (HLS_ESCAPE | HLS_QUOTE | HLS_QMARK_CTRL))
    {
      const char *ptr;

      for (ptr = path; *ptr; ++ptr)
	{
	  const char *add;
	  char buf[5];

	  if (flags & HLS_ESCAPE)
	    {
	      switch (*ptr)
		{
		case '\\':
		  add = "\\\\";
		  break;

		case '\n':
		  add = "\\n";
		  break;

		case '\b':
		  add = "\\b";
		  break;

		case '\r':
		  add = "\\r";
		  break;

		case '\t':
		  add = "\\t";
		  break;

		case '\f':
		  add = "\\f";
		  break;

		case ' ':
		  add = "\\ ";
		  break;

		case '\"':
		  add = "\\\"";
		  break;

		default:
		  if (isgraph(*ptr))
		    {
		      sprintf(buf, "%c", *ptr);
		      add = buf;
		    }
		  else
		    {
		      sprintf(buf, "\\%03o", (unsigned char) *ptr);
		      add = buf;
		    }
		}
	    }
	  else  /* ! (flags & HLS_ESCAPE) */
	    {
	      if (isprint(*ptr) || ! (flags & HLS_QMARK_CTRL))
		{
		  sprintf(buf, "%c", *ptr);
		  add = buf;
		}
	      else
		{
		  sprintf(buf, "?");
		  add = buf;
		}
	    }

	  if (dstr_append(str, add, -1) == -1)
	    return -1;
	}
    }
  else
    {
      if (dstr_append(str, path, -1) == -1)
	return -1;
    }

  if ((flags & HLS_QUOTE) &&
      dstr_append(str, "\"", 1) == -1)
    return -1;

  if (flags & HLS_INDICATOR)
    {
      char c = 0;

      if (ent->dirent.flags & HFS_ISDIR)
	c = ':';
      else if (strcmp(ent->dirent.u.file.type, "APPL") == 0)
	c = '*';

      if (c && dstr_append(str, &c, 1) == -1)
	return -1;
    }

  return 0;
}
