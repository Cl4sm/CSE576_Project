static int
copy_xattrs (const char *temp_name, const char *name, int ignore_errors)
{
  ssize_t sz = listxattr (name, NULL, 0), valsz = 0;
  char *list = NULL, *end, *p, *val = NULL, *newval;

  if (sz < 0)
    {
      if (errno == ENOSYS || errno == ENOTSUP)
	return 0;
      goto read_err;
    }
  list = malloc (sz + 1);
  if (list == NULL)
    goto read_err;
  sz = listxattr (name, list, sz);
  if (sz < 0)
    goto read_err;
  end = list + sz;
  *end = '\0';
  for (p = list; p != end; p = strchr (p, '\0') + 1)
    if (*p == '\0' || strcmp (p, "security.selinux") == 0)
      continue;
    else
      {
	sz = getxattr (name, p, val, valsz);
	if (sz < 0)
	  {
	    if (errno != ERANGE)
	      goto read_err;
	    sz = getxattr (name, p, NULL, 0);
	    if (sz < 0)
	      goto read_err;
	  }
	if (sz > valsz)
	  {
	    valsz = sz * 2;
	    if (valsz < 64)
	      valsz = 64;
	    newval = realloc (val, valsz);
	    if (newval == NULL)
	      goto read_err;
	    val = newval;
	    sz = getxattr (name, p, val, valsz);
	    if (sz < 0)
	      goto read_err;
	  }
	if (setxattr (temp_name, p, val, sz, 0) < 0)
	  {
	    if (errno == ENOSYS || errno == ENOTSUP)
	      continue;
	    if (!ignore_errors)
	      {
		int err = errno;
		ssize_t newsz;

		newval = malloc (sz);
		if (newval == NULL
		    || (newsz = getxattr (temp_name, p, newval, sz)) != sz
		    || memcmp (val, newval, sz) != 0)
		  {
		    error (0, err, "Could not set extended attributes for %s",
			   name);
		    free (newval);
		    free (val);
		    free (list);
		    return 1;
		  }
		free (newval);
	      }
	  }
      }
  free (val);
  free (list);
  return 0;

read_err:
  error (0, errno, "Could not get extended attributes for %s", name);
  free (val);
  free (list);
  return 1;
}
