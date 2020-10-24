int
video_vt_active(int vt)
{
  int fd;
  char buf[16];
  struct vt_stat vtstat;

  int ret;

  ret = snprintf(buf, sizeof(buf), "/dev/tty%d", vt);
  if ((ret < 0) || (ret >= sizeof(buf)))
    return 1;

  /* Try to open the VT the client's X session is running on */
  fd = open(buf, O_RDWR);

  if ((fd < 0) && (errno == EACCES))
    fd = open(buf, O_RDONLY);

  if ((fd < 0) && (errno == EACCES))
    fd = open(buf, O_WRONLY);

  /* Can't open the VT, this shouldn't happen; maybe X is remote? */
  if (fd < 0)
    return 0;

  /* The VT isn't a tty, WTF?! */
  if (!isatty(fd))
    {
      close(fd);
      return 0;
    }

  /* Get VT state, includes currently-active VT number */
  ret = ioctl(fd, VT_GETSTATE, &vtstat);
  close(fd);

  if (ret < 0)
    return 1;

  return (vt == vtstat.v_active);
}