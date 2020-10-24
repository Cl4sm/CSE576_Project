int group_match(uid_t uid, char *groups, gid_t pwent_gid)
{
  int i, j;
  char mtr[32]; // XXX: ...

  memset(mtr, '\0', sizeof mtr);

  for (i = 0, j = 0; i < strlen(groups) + 1; i++) {

    if (groups[i] == ',' || i >= strlen(groups)) {

      if (!check_group(get_user(uid), get_gid(mtr)) || get_gid(mtr) == pwent_gid)
        return 1;

      memset(mtr, '\0', sizeof mtr);
      j = 0;

    } else {

      mtr[j++] = groups[i];

    }

  }

  return 0;
}