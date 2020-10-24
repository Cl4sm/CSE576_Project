     SCM name;
{
  SCM ans = make_vector(MAKINUM(4), UNSPECIFIED);
  struct group *entry;
  SCM *ve = VELTS(ans);
  DEFER_INTS;
  if (UNBNDP(name)) SYSCALL(entry = getgrent(););
  else if (INUMP(name)) SYSCALL(entry = getgrgid(INUM(name)););
  else {
    ASRTER(NIMP(name) && STRINGP(name), name, ARG1, s_grinfo);
    SYSCALL(entry = getgrnam(CHARS(name)););
  }
  ALLOW_INTS;
  if (!entry) return BOOL_F;
  ve[ 0] = makfrom0str(entry->gr_name);
  ve[ 1] = makfrom0str(entry->gr_passwd);
  ve[ 2] = ulong2num((unsigned long)entry->gr_gid);
  ve[ 3] = makfromstrs(-1, entry->gr_mem);
  return ans;
}
