SCM l_hostinfo(name)
     SCM name;
{
  SCM ans = make_vector(MAKINUM(5), UNSPECIFIED);
  SCM *ve = VELTS(ans);
  SCM lst = EOL;
  struct hostent *entry;
  struct in_addr inad;
  const char **argv;
  int i = 0;
# ifndef linux
  if (UNBNDP(name)) {
    DEFER_INTS;
    SYSCALL(entry = gethostent(););
  }
  else
# endif
    if (NIMP(name) && STRINGP(name)) {
    DEFER_INTS;
    SYSCALL(entry = gethostbyname(CHARS(name)););
  }
  else {
    inad.s_addr = htonl(num2ulong(name, (char *)ARG1, s_hostinfo));
    DEFER_INTS;
    SYSCALL(entry = gethostbyaddr((char *)&inad , sizeof(inad), AF_INET););
  }
  ALLOW_INTS;
  if (!entry) return BOOL_F;
  ve[ 0] = makfrom0str(entry->h_name);
  ve[ 1] = makfromstrs(-1, entry->h_aliases);
  ve[ 2] = MAKINUM(entry->h_addrtype + 0L);
  ve[ 3] = MAKINUM(entry->h_length + 0L);
  if (sizeof(struct in_addr) != entry->h_length)
    {ve[ 4] = BOOL_F; return ans;}
  for (argv = entry->h_addr_list; argv[i]; i++);
  while (i--) {
    inad = *(struct in_addr *)argv[i];
    lst = cons(ulong2num(ntohl(inad.s_addr)), lst);
  }
  ve[ 4] = lst;
  return ans;
}
