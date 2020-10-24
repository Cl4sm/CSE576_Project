SCM l_socketpair(fam, proto)
     SCM fam, proto;
{
  int sts, tp = INUM(fam);
  int sv[2];
  FILE* f[2];
  SCM port[2];
  ASRTER(INUMP(fam), fam, ARG1, s_socketpair);
  if (UNBNDP(proto)) proto = INUM0;
  else ASRTER(INUMP(proto), proto, ARG2, s_socketpair);
  NEWCELL(port[0]); NEWCELL(port[1]);
  DEFER_INTS;
  SYSCALL(sts = socketpair(tp, SOCK_STREAM, INUM(proto), sv););
  if (-1==sts) wta(UNDEFINED, (char *)NALLOC, s_socketpair);
  SCM_OPENCALL(f[0] = fdopen(sv[0], "r+"));
  if (!f[0]) {
    close(sv[0]);
    wta(MAKINUM(sv[0]), (char *)NALLOC, s_port_type);
  }
  SCM_OPENCALL(f[1] = fdopen(sv[1], "r+"));
  if (!f[1]) {
    fclose(f[0]);
    close(sv[1]);
    wta(MAKINUM(sv[1]), (char *)NALLOC, s_port_type);
  }
  port[0] = scm_port_entry(f[0], tc16_fport, mode_bits("r+0", (char *)0));
  CAR(port[1]) = scm_port_entry(f[1], tc16_fport, mode_bits("r+0", (char *)0));
  i_setbuf0(port[0]); i_setbuf0(port[1]);
  ALLOW_INTS;
  return cons(port[0], port[1]);
}
