     FILE *f;
{
# ifdef HAVE_SELECT
  fd_set ifds;
  struct timeval tv;
  int ret;

  FD_ZERO(&ifds);
  FD_SET(fileno(f), &ifds);
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  SYSCALL(ret = select((fileno(f) + 1), &ifds, (fd_set *) NULL,
		 (fd_set *) NULL, &tv););
  ASRTER(ret>=0, MAKINUM(ret), "select error", s_char_readyp);
  return FD_ISSET(fileno(f), &ifds);
# else
#  ifdef FIONREAD
  long remir;
  if (feof(f)) return 1;
  ioctl(fileno(f), FIONREAD, &remir);
  return remir;
#  else
  return -1;
#  endif
# endif
}
