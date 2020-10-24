run_time_remap (progname)
     char *progname;
{
  char aout[MAXPATHLEN];
  register char *path, *p;

  /* Just in case */
  if (!initialized)
    return;

  /* Restore the break */
  brk ((char *) brk_value);

  /*  If nothing to remap:  we are done! */
  if (rd_only_len == 0)
    return;

  /*
   * Attempt to find the executable
   * First try argv[0],  will almost always succeed as shells tend to give
   * the full path from the hash list rather than using execvp ()
   */
  if (is_it (progname)) 
    return;

  /*
   * If argv[0] is a full path and does not exist,  not much sense in
   * searching further
   */
  if (strchr (progname, '/')) 
    return;

  /*
   * Try to search for  argv[0] on the PATH
   */
  path = getenv ("PATH");
  if (path == NULL)
    return;

  while (*path)
    {
      /* copy through ':' or end */
      for (p = aout; *p = *path; ++p, ++path)
	if (*p == ':')
	  {
	    ++path;		/* move past ':' */
	    break;
	  }
      *p++ = '/';
      strcpy (p, progname);
      /*
       * aout is a candidate full path name
       */
      if (is_it (aout))
	return;
    }
}
