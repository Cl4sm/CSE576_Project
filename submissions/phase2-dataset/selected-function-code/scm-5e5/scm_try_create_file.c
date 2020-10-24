     SCM fname, modes, perms;
{
  SCM port;
  FILE *f;
  char cmodes[4];
  long flags;
  int fd, fdflags = O_CREAT | O_EXCL;
# ifdef S_IROTH
  mode_t cperms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
# else
  int cperms = 0666;
# endif
  ASRTER(NIMP(fname) && STRINGP(fname), fname, ARG1, s_try_create_file);
  ASRTER(NIMP(modes) && (STRINGP(modes) || SYMBOLP(modes)), modes, ARG2, s_try_create_file);
  if (NNULLP(perms)) {
    perms = CAR(perms);
    ASRTER(INUMP(perms), perms, ARG3, s_try_create_file);
# ifdef S_IROTH
    cperms = (mode_t)INUM(perms);
# else
    cperms = INUM(perms);
# endif
  }
  flags = mode_bits(CHARS(modes), cmodes);
  ASRTER(flags, modes, ARG2, s_try_create_file);
  fdflags |= (RDNG & flags) ? O_RDWR : O_WRONLY;
  DEFER_INTS;
  SCM_OPENCALL(fd = open(CHARS(fname), fdflags, cperms));
  if (fd >= 0 && (f = fdopen(fd, cmodes))) {
    port = scm_port_entry(f, tc16_fport, flags);
    if (BUF0 & flags) i_setbuf0(port);
    SCM_PORTDATA(port) = fname;
  }
  else
    port = BOOL_F;
  ALLOW_INTS;
  return port;
}
