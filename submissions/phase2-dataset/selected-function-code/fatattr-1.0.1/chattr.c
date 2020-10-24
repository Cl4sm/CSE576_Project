static int
chattr(const char *file, uint32_t s_attr, uint32_t c_attr)
{
  int fd = -1;
  uint32_t attr, nattr;
  int e;

  fd = open(file, O_RDONLY);
  if ( fd < 0 )
    goto err;

  if ( ioctl(fd, FAT_IOCTL_GET_ATTRIBUTES, &attr) )
    goto err;

  nattr = (attr & ~c_attr) | s_attr;

  if ( nattr != attr ) {
    if ( ioctl(fd, FAT_IOCTL_SET_ATTRIBUTES, &nattr) )
      goto err;
  }

  close(fd);
  return 0;

 err:
  e = errno;
  if ( fd >= 0 )
    close(fd);

  errno = e;
  return -1;
}