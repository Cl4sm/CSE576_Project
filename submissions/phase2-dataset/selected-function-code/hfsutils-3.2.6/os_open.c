{
  int fd;
  struct flock lock;

  switch (mode)
    {
    case HFS_MODE_RDONLY:
      mode = O_RDONLY;
      break;

    case HFS_MODE_RDWR:
    default:
      mode = O_RDWR;
      break;
    }

  fd = open(path, mode);
  if (fd == -1)
    ERROR(errno, "error opening medium");

  /* lock descriptor against concurrent access */

  lock.l_type   = (mode == O_RDONLY) ? F_RDLCK : F_WRLCK;
  lock.l_start  = 0;
  lock.l_whence = SEEK_SET;
  lock.l_len    = 0;

  if (fcntl(fd, F_SETLK, &lock) == -1 &&
      (errno == EACCES || errno == EAGAIN))
    ERROR(EAGAIN, "unable to obtain lock for medium");

  *priv = (void *) fd;

  return 0;

fail:
  if (fd != -1)
    close(fd);

  return -1;
}
