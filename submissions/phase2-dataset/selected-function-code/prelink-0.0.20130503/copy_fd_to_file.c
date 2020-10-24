copy_fd_to_file (int fdin, const char *name, struct stat64 *st)
{
  struct stat64 stt;
  off_t off = 0;
  int err, fdout;
  struct utimbuf u;

  if (strcmp (name, "-") == 0)
    fdout = 1;
  else
    fdout = open (name, O_WRONLY | O_CREAT, 0600);
  if (fdout != -1
      && fstat64 (fdin, &stt) >= 0
      && send_file (fdout, fdin, &off, stt.st_size) == stt.st_size)
    {
      if (fchown (fdout, st->st_uid, st->st_gid) >= 0)
	fchmod (fdout, st->st_mode & 07777);
      if (strcmp (name, "-") != 0)
	{
	  set_security_context (name, name, 1);
	  u.actime = time (NULL);
	  u.modtime = st->st_mtime;
	  utime (name, &u);
	  close (fdout);
	}
      return 0;
    }
  else if (fdout != -1)
    {
      err = errno;
      if (strcmp (name, "-") == 0)
	close (fdout);
    }
  else
    err = errno;
  return err;
}
