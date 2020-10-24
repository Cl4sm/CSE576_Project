static int
lsattr(const char *file)
{
  int fd = -1;
  uint32_t attr;
  int i, e;

  fd = open(file, O_RDONLY);
  if ( fd < 0 )
    goto err;

  if ( ioctl(fd, FAT_IOCTL_GET_ATTRIBUTES, &attr) )
    goto err;

  close(fd);

  for ( i = 0 ; bit_to_char[i] ; i++ ) {
    putchar( (attr & 1) ? bit_to_char[i] : ' ' );
    attr >>= 1;
  }

  putchar(' ');
  puts(file);
  return 0;

 err:
  e = errno;
  if ( fd >= 0 )
    close(fd);
  errno = e;
  return -1;
}