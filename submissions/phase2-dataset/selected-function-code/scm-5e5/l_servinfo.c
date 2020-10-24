SCM l_servinfo(args)
     SCM args;
{
  SCM ans = make_vector(MAKINUM(4), UNSPECIFIED);
  SCM *ve = VELTS(ans);
  SCM name, proto;
  struct servent *entry;
  if (NULLP(args)) {
    DEFER_INTS;
    SYSCALL(entry = getservent(););
    goto comlab;
  }
  name = CAR(args);
  proto = CDR(args);
  ASRTER(NIMP(proto) && CONSP(proto), args, WNA, s_servinfo);
  proto = CAR(proto);
  ASRTER(NIMP(proto) && STRINGP(proto), args, ARG2, s_servinfo);
  DEFER_INTS;
  if (NIMP(name) && STRINGP(name)) {
    SYSCALL(entry = getservbyname(CHARS(name), CHARS(proto)););
  }
  else {
    ASRTER(INUMP(proto), proto, ARG1, s_servinfo);
    SYSCALL(entry = getservbyport(INUM(proto), CHARS(proto)););
  }
 comlab:  ALLOW_INTS;
  if (!entry) return BOOL_F;
  ve[ 0] = makfrom0str(entry->s_name);
  ve[ 1] = makfromstrs(-1, entry->s_aliases);
  ve[ 2] = MAKINUM(ntohs(entry->s_port) + 0L);
  ve[ 3] = makfrom0str(entry->s_proto);
  return ans;
}
