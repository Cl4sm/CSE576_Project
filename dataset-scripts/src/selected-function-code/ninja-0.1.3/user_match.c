int user_match(char *item, char *users)
{
  int i, j;
  int match = 0;
  char mtr[32]; // XXX: ...

  memset(mtr, '\0', sizeof mtr);

  for (i = 0, j = 0; i < strlen(users) + 1; i++) {

    if (users[i] == ',' || i >= strlen(users)) {

      if (!strcmp(mtr, item))
        match++;

      memset(mtr, '\0', sizeof mtr);
      j = 0;

    } else {

      mtr[j++] = users[i];

    }

  }

  return match;
}