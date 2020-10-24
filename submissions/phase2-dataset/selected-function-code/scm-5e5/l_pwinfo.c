SCM l_pwinfo(user)
     SCM user;
{
  SCM ans = make_vector(MAKINUM(7), UNSPECIFIED);
  struct passwd *entry;
  SCM *ve = VELTS(ans);
  DEFER_INTS;
  if (UNBNDP(user)) SYSCALL(entry = getpwent(););
  else if (INUMP(user)) SYSCALL(entry = getpwuid(INUM(user)););
  else {
    ASRTER(NIMP(user) && STRINGP(user), user, ARG1, s_pwinfo);
    SYSCALL(entry = getpwnam(CHARS(user)););
  }
  ALLOW_INTS;
  if (!entry) return BOOL_F;
  ve[ 0] = makfrom0str(entry->pw_name);
  ve[ 1] = makfrom0str(entry->pw_passwd);
  ve[ 2] = ulong2num((unsigned long)entry->pw_uid);
  ve[ 3] = ulong2num((unsigned long)entry->pw_gid);
  ve[ 4] = makfrom0str(entry->pw_gecos);
  ve[ 5] = makfrom0str(entry->pw_dir);
  ve[ 6] = makfrom0str(entry->pw_shell);
  return ans;
}
