static char *
canon_filename (const char *name, int nested, struct stat64 *stp)
{
  char *rpath, *dest, *extra_buf = NULL;
  const char *start, *end, *rpath_limit;
  long int path_max;
  int num_links = 0;
  int stp_initialized = 0;

  if (name == NULL)
    {
      errno = EINVAL;
      return NULL;
    }

  if (name[0] == '\0')
    {
      errno = ENOENT;
      return NULL;
    }

#ifdef PATH_MAX
  path_max = PATH_MAX;
#else
  path_max = pathconf (name, _PC_PATH_MAX);
  if (path_max <= 0)
    path_max = 1024;
#endif

  rpath = malloc (path_max);
  if (rpath == NULL)
    return NULL;
  rpath_limit = rpath + path_max;

  if (name[0] != '/')
    {
      if (!getcwd (rpath, path_max))
	{
	  rpath[0] = '\0';
	  goto error;
	}
      dest = strchr (rpath, '\0');
    }
  else
    {
      rpath[0] = '/';
      dest = rpath + 1;

      if (!nested)
	{
	  if (prelink_dirname_htab == NULL)
	    prelink_dirname_htab = htab_try_create (100, dirname_hash,
						    dirname_eq, NULL);
	  if (prelink_dirname_htab == NULL)
	    nested = 1;
	}
      if (!nested)
	{
	  struct dirname_entry e;
	  void **dirname_slot;

	  end = strrchr (name, '/');

	  e.dirname = name;
	  e.dirname_len = end - name;
	  dirname_slot = htab_find_slot (prelink_dirname_htab, &e, INSERT);
	  if (*dirname_slot == NULL)
	    {
	      struct dirname_entry *ep = malloc (sizeof (struct dirname_entry)
						 + e.dirname_len + 1);
	      if (ep != NULL)
		{
		  char *dirname = (char *) (ep + 1);
		  struct stat64 st;

		  ep->dirname = (const char *) dirname;
		  ep->dirname_len = e.dirname_len;
		  memcpy (dirname, name, ep->dirname_len);
		  dirname[ep->dirname_len] = '\0';
		  ep->canon_dirname = canon_filename (ep->dirname, 1, &st);
		  if (ep->canon_dirname == NULL || !S_ISDIR (st.st_mode))
		    free (ep);
		  else
		    {
		      ep->canon_dirname_len = strlen (ep->canon_dirname);
		      *dirname_slot = ep;
		    }
		}
	    }

	  if (*dirname_slot != NULL)
	    {
	      struct dirname_entry *ep = *dirname_slot;

	      if (rpath + ep->canon_dirname_len + 1 >= rpath_limit)
		{
		  size_t new_size;
		  char *new_rpath;

		  new_size = rpath_limit - rpath;
		  if (ep->canon_dirname_len + 1 > path_max)
		    new_size += ep->canon_dirname_len + 1;
		  else
		    new_size += path_max;
		  new_rpath = (char *) realloc (rpath, new_size);
		  if (new_rpath == NULL)
		    goto error;
		  rpath = new_rpath;
		  rpath_limit = rpath + new_size;
		}
	      dest = mempcpy (rpath, ep->canon_dirname, ep->canon_dirname_len);
	      *dest = '\0';
	      name = end + 1;
	    }
	}
    }

  for (start = end = name; *start; start = end)
    {
      int n;

      /* Skip sequence of multiple path-separators.  */
      while (*start == '/')
	++start;

      /* Find end of path component.  */
      for (end = start; *end && *end != '/'; ++end)
	/* Nothing.  */;

      if (end - start == 0)
	break;
      else if (end - start == 1 && start[0] == '.')
	/* nothing */;
      else if (end - start == 2 && start[0] == '.' && start[1] == '.')
	{
	  /* Back up to previous component, ignore if at root already.  */
	  if (dest > rpath + 1)
	    while ((--dest)[-1] != '/');
	  stp_initialized = 0;
	}
      else
	{
	  size_t new_size;

	  if (dest[-1] != '/')
	    *dest++ = '/';

	  if (dest + (end - start) >= rpath_limit)
	    {
	      ptrdiff_t dest_offset = dest - rpath;
	      char *new_rpath;

	      new_size = rpath_limit - rpath;
	      if (end - start + 1 > path_max)
		new_size += end - start + 1;
	      else
		new_size += path_max;
	      new_rpath = (char *) realloc (rpath, new_size);
	      if (new_rpath == NULL)
		goto error;
	      rpath = new_rpath;
	      rpath_limit = rpath + new_size;

	      dest = rpath + dest_offset;
	    }

	  dest = mempcpy (dest, start, end - start);
	  *dest = '\0';

	  if (lstat64 (rpath, stp) < 0)
	    goto error;

	  stp_initialized = 1;

	  if (S_ISLNK (stp->st_mode))
	    {
	      char *buf = alloca (path_max);
	      size_t len;

	      if (++num_links > MAXSYMLINKS)
		{
		  errno = ELOOP;
		  goto error;
		}

	      n = readlink (rpath, buf, path_max);
	      if (n < 0)
		goto error;
	      buf[n] = '\0';

	      if (!extra_buf)
		extra_buf = alloca (path_max);

	      len = strlen (end);
	      if ((long int) (n + len) >= path_max)
		{
		  errno = ENAMETOOLONG;
		  goto error;
		}

	      /* Careful here, end may be a pointer into extra_buf... */
	      memmove (&extra_buf[n], end, len + 1);
	      name = end = memcpy (extra_buf, buf, n);

	      if (buf[0] == '/')
		dest = rpath + 1;	/* It's an absolute symlink */
	      else
		/* Back up to previous component, ignore if at root already: */
		if (dest > rpath + 1)
		  while ((--dest)[-1] != '/');
	    }
	  else if (!S_ISDIR (stp->st_mode) && *end != '\0')
	    {
	      errno = ENOTDIR;
	      goto error;
	    }
	}
    }
  if (dest > rpath + 1 && dest[-1] == '/')
    --dest;
  *dest = '\0';

  if (!stp_initialized && lstat64 (rpath, stp) < 0)
    goto error;

  if (dest + 1 - rpath <= (rpath_limit - rpath) / 2)
    {
      char *new_rpath = realloc (rpath, dest + 1 - rpath);

      if (new_rpath != NULL)
	return new_rpath;
    }
  return rpath;

error:
  free (rpath);
  return NULL;
}
