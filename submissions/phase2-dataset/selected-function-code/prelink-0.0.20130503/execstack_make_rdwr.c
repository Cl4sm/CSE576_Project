static int
execstack_make_rdwr (DSO *dso, int flag)
{
  int i, fd = -1, status;
  pid_t pid;
  DSO *ndso = NULL;
  char *p = NULL;
  char filename[strlen (dso->filename) + sizeof ".#execstack#.XXXXXX"];

  for (i = 0; i < dso->ehdr.e_shnum; ++i)
    {
      const char *name = strptr (dso, dso->ehdr.e_shstrndx,
				 dso->shdr[i].sh_name);
      if (strcmp (name, ".gnu.prelink_undo") == 0)
	break;
    }

  if (i == dso->ehdr.e_shnum)
    return reopen_dso (dso, NULL, NULL) ? 1 : -1;

  /* We need to unprelink the file first, so that prelink --undo
     or reprelinking it doesn't destroy the PT_GNU_STACK segment
     header we've created.  */
  sprintf (filename, "%s.#execstack#.XXXXXX", dso->filename);

  fd = mkstemp (filename);
  if (fd == -1)
    {
      error (0, 0, "%s: Cannot create temporary file",
	     dso->filename);
      goto error_out;
    }

  p = strdup (dso->filename);
  if (p == NULL)
    {
      error (0, ENOMEM, "%s: Cannot create temporary file",
	     dso->filename);
      goto error_out;
    }

  pid = vfork ();
  if (pid == 0)
    {
      close (fd);
      execlp ("prelink", "prelink", "-u", "-o", filename,
	      dso->filename, NULL);
      execl (SBINDIR "/prelink", "prelink", "-u", "-o", filename,
	     dso->filename, NULL);
      _exit (-1);
    }

  if (pid < 0)
    {
      error (0, errno, "%s: Cannot run prelink --undo",
	     dso->filename);
      goto error_out;
    }

  if (waitpid (pid, &status, 0) < 0
      || !WIFEXITED (status)
      || WEXITSTATUS (status))
    {
      error (0, 0, "%s: prelink --undo failed", dso->filename);
      goto error_out;
    }

  ndso = open_dso (filename);
  if (ndso == NULL)
    {
      error (0, 0, "%s: Couldn't open prelink --undo output",
	     dso->filename);
      goto error_out;
    }

  for (i = 0; i < ndso->ehdr.e_shnum; ++i)
    {
      const char *name = strptr (ndso, ndso->ehdr.e_shstrndx,
				 ndso->shdr[i].sh_name);
      if (strcmp (name, ".gnu.prelink_undo") == 0)
	break;
    }

  if (i != ndso->ehdr.e_shnum)
    {
      error (0, 0, "%s: prelink --undo output contains .gnu.prelink_undo section",
	     dso->filename);
      goto error_out;
    }

  if (ndso->ehdr.e_type != dso->ehdr.e_type)
    {
      error (0, 0, "%s: Object type changed during prelink --undo operation",
	     dso->filename);
    }

  if (ndso->filename != ndso->soname)
    free ((char *) ndso->filename);
  ndso->filename = p;
  p = NULL;

  unlink (filename);
  close (fd);
  fd = -1;
  close_dso (dso);
  return execstack_set (ndso, flag);

error_out:
  free (p);
  if (ndso != NULL)
    close_dso (ndso);
  if (fd != -1)
    {
      unlink (filename);
      close (fd);
    }
  close_dso (dso);
  return 1;
}
