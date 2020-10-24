void
ambient_get(int *r, int *l)
{
  int fd;
  int ret;
  char buf[16];
  char *p;

  if (!smcpath)
    goto out_error;

  fd = open(smcpath, O_RDONLY);
  if (fd < 0)
    goto out_error;

  ret = read(fd, buf, 16);

  close(fd);

  if ((ret <= 0) || (ret > 15))
    goto out_error;

  buf[strlen(buf)] = '\0';

  p = strchr(buf, ',');
  *p++ = '\0';
  *r = atoi(p);

  p = buf + 1;
  *l = atoi(p);

  logdebug("Ambient light: right %d, left %d\n", *r, *l);

  ambient_info.right = *r;
  ambient_info.left = *l;

  return;

 out_error:
  *r = -1;
  *l = -1;

  ambient_info.right = 0;
  ambient_info.left = 0;
}