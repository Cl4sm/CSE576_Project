int
update_dso (DSO *dso, const char *orig_name)
{
  int rdwr = dso_is_rdwr (dso);

  if (rdwr)
    {
      char *name1, *name2;
      struct utimbuf u;
      struct stat64 st;
      int fdin;

      switch (write_dso (dso))
	{
	case 2:
	  error (0, 0, "Could not write %s: %s", dso->filename,
		 elf_errmsg (-1));
	  /* FALLTHROUGH */
	case 1:
	  close_dso (dso);
	  return 1;
	case 0:
	  break;
	}

      name1 = strdupa (dso->filename);
      name2 = strdupa (dso->temp_filename);
      if (fstat64 (dso->fdro, &st) < 0)
	{
	  error (0, errno, "Could not stat %s", dso->filename);
	  close_dso (dso);
	  return 1;
	}
      if ((fchown (dso->fd, st.st_uid, st.st_gid) < 0
	   || fchmod (dso->fd, st.st_mode & 07777) < 0)
	  && orig_name == NULL)
	{
	  error (0, errno, "Could not set %s owner or mode", dso->filename);
	  close_dso (dso);
	  return 1;
	}
      if (orig_name != NULL)
	fdin = dup (dso->fd);
      else
	fdin = -1;
      close_dso_1 (dso);
      u.actime = time (NULL);
      u.modtime = st.st_mtime;
      utime (name2, &u);

      if (set_security_context (name2, orig_name ? orig_name : name1,
				orig_name != NULL))
	{
	  if (fdin != -1)
	    close (fdin);
	  unlink (name2);
	  return 1;
	}

      if ((orig_name != NULL && strcmp (name1, "-") == 0)
	  || rename (name2, name1))
	{
	  if (fdin != -1)
	    {
	      int err = copy_fd_to_file (fdin, name1, &st);

	      close (fdin);
	      unlink (name2);
	      if (err == 0)
		return 0;
	      error (0, err, "Could not rename nor copy temporary to %s",
		     name1);
	      return 1;
	    }
	  unlink (name2);
	  error (0, errno, "Could not rename temporary to %s", name1);
	  return 1;
	}
      if (fdin != -1)
	close (fdin);
    }
  else
    close_dso_1 (dso);

  return 0;
}
