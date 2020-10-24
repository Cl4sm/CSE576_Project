int check_group(const char *user, const gid_t gid)
{
  size_t group_count;
  gid_t gid_list[NGROUPS_MAX+1];
  int i;  

  if (user == NULL)
    return 1;

  initgroups(user, -1);

  group_count = getgroups(0, gid_list);
  getgroups(group_count, gid_list);

  for (i = 0; i < group_count; i++) {
    if (gid_list[i] == gid)
      return 0;
  }

  return 1;
}