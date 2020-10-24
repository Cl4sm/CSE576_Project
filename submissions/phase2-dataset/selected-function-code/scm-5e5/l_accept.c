     SCM sockpt;
{
  int newsd, sadlen;
  struct sockaddr sad;
  FILE *newfd;
  SCM newpt;
  NEWCELL(newpt);
  ASRTER(NIMP(sockpt) && OPINPORTP(sockpt), sockpt, ARG1, s_accept);
  sadlen=sizeof(sad);
  SYSCALL(newsd = accept(fileno(STREAM(sockpt)), &sad, &sadlen););
  if (-1==newsd) {
#ifndef macintosh
    if (EWOULDBLOCK != errno) return BOOL_F;
    else
#endif
        wta(sockpt, "couldn't", s_accept);
  }
  DEFER_INTS;
  SCM_OPENCALL(newfd = fdopen(newsd, "r+"));
  if (!newfd) {
    close(newsd);
    wta(MAKINUM(newsd), (char *)NALLOC, s_port_type);
  }
  newpt = scm_port_entry(newfd, tc16_fport, mode_bits("r+0", (char *)0));
  i_setbuf0(newpt);
  ALLOW_INTS;
  return newpt;
}
