SCM l_protoinfo(name)
     SCM name;
{
  SCM ans = make_vector(MAKINUM(3), UNSPECIFIED);
  SCM *ve = VELTS(ans);
  struct protoent *entry;
  if (UNBNDP(name)) {
    DEFER_INTS;
    SYSCALL(entry = getprotoent(););
  }
  else if (NIMP(name) && STRINGP(name)) {
    DEFER_INTS;
    SYSCALL(entry = getprotobyname(CHARS(name)););
  }
  else {
    unsigned long protonum;
    protonum = num2ulong(name, (char *)ARG1, s_protoinfo);
    DEFER_INTS;
    SYSCALL(entry = getprotobynumber(protonum););
  }
  ALLOW_INTS;
  if (!entry) return BOOL_F;
  ve[ 0] = makfrom0str(entry->p_name);
  ve[ 1] = makfromstrs(-1, entry->p_aliases);
  ve[ 2] = MAKINUM(entry->p_proto + 0L);
  return ans;
}
