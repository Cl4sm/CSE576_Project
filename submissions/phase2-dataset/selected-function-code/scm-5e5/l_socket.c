     SCM fam, proto;
{
  int sd, j, tp = INUM(fam);
  FILE* f;
  SCM port;
  ASRTER(INUMP(fam), fam, ARG1, s_socket);
  if (UNBNDP(proto)) proto = INUM0;
  else ASRTER(INUMP(proto), proto, ARG2, s_socket);
  NEWCELL(port);
  DEFER_INTS;
  SYSCALL(sd = socket(tp, SOCK_STREAM, INUM(proto)););
  if (-1==sd) wta(UNDEFINED, (char *)NALLOC, s_socket);
  SYSCALL(f = fdopen(sd, "r+"););
  /*  SCM_OPENCALL(f = fdopen(sd, "r+")); */
  if (!f) {
    close(sd);
    wta(MAKINUM(sd), (char *)NALLOC, s_port_type);
  }
  port = scm_port_entry(f, tc_socket, BUF0);
  SCM_PORTDATA(port) = fam;
  i_setbuf0(port);
  ALLOW_INTS;
  if (AF_INET==tp) {
#ifdef macintosh
    sd = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&j, sizeof(j));
#else
    sd = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &j, sizeof(j));
#endif
    ASRTER(!sd, port, "could not set socket option", s_socket);
  }
  return port;
}
