internal_function
read_alias_file (fname, fname_len)
     const char *fname;
     int fname_len;
{
  FILE *fp;
  char *full_fname;
  size_t added;
  static const char aliasfile[] = "/locale.alias";

  full_fname = (char *) alloca (fname_len + sizeof aliasfile);
#ifdef HAVE_MEMPCPY
  mempcpy (mempcpy (full_fname, fname, fname_len),
	   aliasfile, sizeof aliasfile);
#else
  memcpy (full_fname, fname, fname_len);
  memcpy (&full_fname[fname_len], aliasfile, sizeof aliasfile);
#endif

  fp = fopen (relocate (full_fname), "r");
  freea (full_fname);
  if (fp == NULL)
    return 0;

#ifdef HAVE___FSETLOCKING
  /* No threads present.  */
  __fsetlocking (fp, FSETLOCKING_BYCALLER);
#endif

  added = 0;
  while (!FEOF (fp))
    {
      /* It is a reasonable approach to use a fix buffer here because
	 a) we are only interested in the first two fields
	 b) these fields must be usable as file names and so must not
	    be that long
	 We avoid a multi-kilobyte buffer here since this would use up
	 stack space which we might not have if the program ran out of
	 memory.  */
      char buf[400];
      char *alias;
      char *value;
      char *cp;

      if (FGETS (buf, sizeof buf, fp) == NULL)
	/* EOF reached.  */
	break;

      cp = buf;
      /* Ignore leading white space.  */
      while (isspace ((unsigned char) cp[0]))
	++cp;

      /* A leading '#' signals a comment line.  */
      if (cp[0] != '\0' && cp[0] != '#')
	{
	  alias = cp++;
	  while (cp[0] != '\0' && !isspace ((unsigned char) cp[0]))
	    ++cp;
	  /* Terminate alias name.  */
	  if (cp[0] != '\0')
	    *cp++ = '\0';

	  /* Now look for the beginning of the value.  */
	  while (isspace ((unsigned char) cp[0]))
	    ++cp;

	  if (cp[0] != '\0')
	    {
	      size_t alias_len;
	      size_t value_len;

	      value = cp++;
	      while (cp[0] != '\0' && !isspace ((unsigned char) cp[0]))
		++cp;
	      /* Terminate value.  */
	      if (cp[0] == '\n')
		{
		  /* This has to be done to make the following test
		     for the end of line possible.  We are looking for
		     the terminating '\n' which do not overwrite here.  */
		  *cp++ = '\0';
		  *cp = '\n';
		}
	      else if (cp[0] != '\0')
		*cp++ = '\0';

	      if (nmap >= maxmap)
		if (__builtin_expect (extend_alias_table (), 0))
		  return added;

	      alias_len = strlen (alias) + 1;
	      value_len = strlen (value) + 1;

	      if (string_space_act + alias_len + value_len > string_space_max)
		{
		  /* Increase size of memory pool.  */
		  size_t new_size = (string_space_max
				     + (alias_len + value_len > 1024
					? alias_len + value_len : 1024));
		  char *new_pool = (char *) realloc (string_space, new_size);
		  if (new_pool == NULL)
		    return added;

		  if (__builtin_expect (string_space != new_pool, 0))
		    {
		      size_t i;

		      for (i = 0; i < nmap; i++)
			{
			  map[i].alias += new_pool - string_space;
			  map[i].value += new_pool - string_space;
			}
		    }

		  string_space = new_pool;
		  string_space_max = new_size;
		}

	      map[nmap].alias = memcpy (&string_space[string_space_act],
					alias, alias_len);
	      string_space_act += alias_len;

	      map[nmap].value = memcpy (&string_space[string_space_act],
					value, value_len);
	      string_space_act += value_len;

	      ++nmap;
	      ++added;
	    }
	}

      /* Possibly not the whole line fits into the buffer.  Ignore
	 the rest of the line.  */
      while (strchr (buf, '\n') == NULL)
	if (FGETS (buf, sizeof buf, fp) == NULL)
	  /* Make sure the inner loop will be left.  The outer loop
	     will exit at the `feof' test.  */
	  break;
    }

  /* Should we test for ferror()?  I think we have to silently ignore
     errors.  --drepper  */
  fclose (fp);

  if (added > 0)
    qsort (map, nmap, sizeof (struct alias_map),
	   (int (*) PARAMS ((const void *, const void *))) alias_compare);

  return added;
}
