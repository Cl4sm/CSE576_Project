SCM l_dup(oldpt, modes)
     SCM oldpt, modes;
{
  long flags;
  char cmodes[4];
  int tfd;
  FILE *f;
  SCM newpt;
  ASRTER(NIMP(oldpt) && OPFPORTP(oldpt), oldpt, ARG1, s_dup);
  ASRTER(NIMP(modes) && (STRINGP(modes) || SYMBOLP(modes)), modes, ARG2, s_dup);
  flags = mode_bits(CHARS(modes), cmodes);
  ASRTER(flags, modes, ARG2, s_dup);
  NEWCELL(newpt);
  DEFER_INTS;
  SCM_OPENCALL(tfd = dup(fileno(STREAM(oldpt))));
  if (-1==tfd) {ALLOW_INTS;return BOOL_F;};
  SYSCALL(f = fdopen(tfd, cmodes););
  if (!f) {
    int lerrno = errno;
    close(tfd);
    errno = lerrno;
#  ifdef EINVAL
    if (lerrno==EINVAL) wta(modes, (char *)ARG2, s_dup);
#  endif
    wta(MAKINUM(tfd), (char *)NALLOC, s_port_type);
  }
  newpt = scm_port_entry(f, tc16_fport, flags);
  SCM_PORTDATA(newpt) = SCM_PORTDATA(oldpt);
  if (BUF0 & flags)
    i_setbuf0(newpt);
  ALLOW_INTS;
  return newpt;
}
