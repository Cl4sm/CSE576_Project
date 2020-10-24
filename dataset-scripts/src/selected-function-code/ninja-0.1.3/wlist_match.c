int wlist_match(char *wlist, pid_t pid, uid_t uid, gid_t gid)
{
  unsigned long wlist_pos = 0;
  int i = 0;

#ifdef USE_READLINK
  char map_name[PATH_MAX+8];
  char link_path[64];
#else
  char *map_name;
#endif

  char wlist_entry[512];
  char wlist_exec[512];
  char wlist_groups[512];
  char wlist_users[512];

#ifdef USE_READLINK
  memset(map_name, '\0', sizeof map_name);
  snprintf(link_path, sizeof link_path, "/proc/%d/exe", pid);
  if (readlink(link_path, map_name, sizeof map_name - 1) == -1)
    return 0;
#else
  if ((map_name = get_map_name(pid)) == NULL)
    return 0;
#endif

  for (;;) {

    wlist_entry[i++] = wlist[wlist_pos++];

    if (wlist[wlist_pos] == '\0')
      return 0;

    if (wlist[wlist_pos] == '\n') {
      wlist_entry[i] = '\0';

      memset(wlist_exec, '\0', sizeof wlist_exec);
      memset(wlist_groups, '\0', sizeof wlist_groups);
      memset(wlist_users, '\0', sizeof wlist_users);

      chreplace(wlist_entry, ':', ' ');
      sscanf(wlist_entry, "%511s %511s %511s", wlist_exec, wlist_groups, wlist_users);

      /* map name (executable) matces */
      if (!strcmp(map_name, wlist_exec)) {

        /* check users */

        if (user_match(get_user(uid), wlist_users) > 0)
          return 1;

        /* check groups */

        if (group_match(uid, wlist_groups, gid) > 0)
          return 1;

      }

      i = 0;
      wlist_pos++;

      if (wlist_pos >= strlen(wlist))
        break;

    }

  }

  return 0;
}