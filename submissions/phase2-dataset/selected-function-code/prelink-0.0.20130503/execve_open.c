FILE *
execve_open (const char *path, char *const argv[], char *const envp[])
{
  int p[2];
  FILE *f;

  if (pipe (p) < 0)
    {
      error (0, errno, "Could not run %s", path);
      return NULL;
    }

  switch (vfork ())
    {
    case -1:
      error (0, errno, "Could not run %s", path);
      return NULL;
    case 0:
      close (p[0]);
      if (p[1] != 1)
	{
	  dup2 (p[1], 1);
	  close (p[1]);
	}
      dup2 (1, 2);
      execve (path, argv, envp);
      _exit (127);
    }

  close (p[1]);

  f = fdopen (p[0], "r");
  if (f == NULL)
    {
      close (p[0]);
      execve_close (NULL);
    }

  return f;
}
