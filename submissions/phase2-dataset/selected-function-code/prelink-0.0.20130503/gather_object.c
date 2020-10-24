int
gather_object (const char *name, int deref, int onefs)
{
  struct stat64 st;

  if (stat64 (name, &st) < 0)
    {
      if (implicit)
	return 0;
      error (0, errno, "Could not stat %s", name);
      return 1;
    }

  if (S_ISDIR (st.st_mode))
    {
      int flags = 0, ret;
      if (! deref) flags |= FTW_PHYS;
      if (onefs) flags |= FTW_MOUNT;

      if (implicit && ! deref)
	{
	  ret = add_dir_to_dirlist (name, st.st_dev, flags);
	  if (ret)
	    return ret == 2 ? 0 : 1;
	}
      if (!all && implicit && ! deref)
	return 0;
      ++implicit;
      ret = nftw64 (name, gather_func, 20, flags | FTW_ACTIONRETVAL);
      --implicit;
      if (ret < 0)
	error (0, errno, "Failed searching %s", name);
#ifndef HAVE_FTW_ACTIONRETVAL
      free (blacklist_dir);
      blacklist_dir = NULL;
#endif
      return ret;
    }
  else
    return gather_binlib (name, &st);
}
