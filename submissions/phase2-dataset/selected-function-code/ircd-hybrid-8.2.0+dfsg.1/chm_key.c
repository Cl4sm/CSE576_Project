chm_key(struct Client *source_p, struct Channel *chptr, int parc, int *parn,
        char **parv, int *errors, int alev, int dir, char c, unsigned int d)
{
  if (alev < CHACCESS_HALFOP)
  {
    if (!(*errors & SM_ERR_NOOPS))
      sendto_one_numeric(source_p, &me,
                         alev == CHACCESS_NOTONCHAN ? ERR_NOTONCHANNEL :
                         ERR_CHANOPRIVSNEEDED, chptr->chname);
    *errors |= SM_ERR_NOOPS;
    return;
  }

  if (dir == MODE_QUERY)
    return;

  if (dir == MODE_ADD && parc > *parn)
  {
    char *key = fix_key(parv[(*parn)++]);

    if (EmptyString(key))
      return;

    assert(key[0] != ' ');
    strlcpy(chptr->mode.key, key, sizeof(chptr->mode.key));

    /* If somebody does MODE #channel +kk a b, accept latter --fl */
    for (unsigned int i = 0; i < mode_count; ++i)
      if (mode_changes[i].letter == c && mode_changes[i].dir == MODE_ADD)
        mode_changes[i].letter = 0;

    mode_changes[mode_count].letter = c;
    mode_changes[mode_count].arg = chptr->mode.key;
    mode_changes[mode_count].id = NULL;
    mode_changes[mode_count].dir = dir;
    mode_changes[mode_count++].mems = ALL_MEMBERS;
  }
  else if (dir == MODE_DEL)
  {
    if (parc > *parn)
      (*parn)++;

    if (chptr->mode.key[0] == '\0')
      return;

    chptr->mode.key[0] = '\0';

    mode_changes[mode_count].letter = c;
    mode_changes[mode_count].arg = "*";
    mode_changes[mode_count].id = NULL;
    mode_changes[mode_count].dir = dir;
    mode_changes[mode_count++].mems = ALL_MEMBERS;
  }
}
