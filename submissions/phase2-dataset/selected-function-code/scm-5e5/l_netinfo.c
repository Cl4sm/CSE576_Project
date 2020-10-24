     SCM name;
{
  SCM ans = make_vector(MAKINUM(4), UNSPECIFIED);
  SCM *ve = VELTS(ans);
  struct netent *entry;
  if (UNBNDP(name)) {
    DEFER_INTS;
    SYSCALL(entry = getnetent(););
  }
  else if (NIMP(name) && STRINGP(name)) {
    DEFER_INTS;
    SYSCALL(entry = getnetbyname(CHARS(name)););
  }
  else {
    unsigned long netnum;
    netnum = num2ulong(name, (char *)ARG1, s_netinfo);
    DEFER_INTS;
    SYSCALL(entry = getnetbyaddr(netnum, AF_INET););
  }
  ALLOW_INTS;
  if (!entry) return BOOL_F;
  ve[ 0] = makfrom0str(entry->n_name);
  ve[ 1] = makfromstrs(-1, entry->n_aliases);
  ve[ 2] = MAKINUM(entry->n_addrtype + 0L);
  ve[ 3] = ulong2num(entry->n_net + 0L);
  return ans;
}
