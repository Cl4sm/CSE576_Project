chm_limit(struct Client *source_p, struct Channel *chptr, int parc, int *parn,
          char **parv, int *errors, int alev, int dir, char c, unsigned int d)
{
  int limit = 0;

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
    char *lstr = parv[(*parn)++];

    if (EmptyString(lstr) || (limit = atoi(lstr)) <= 0)
      return;

    sprintf(lstr, "%d", limit);

    /* If somebody sets MODE #channel +ll 1 2, accept latter --fl */
    for (unsigned int i = 0; i < mode_count; ++i)
      if (mode_changes[i].letter == c && mode_changes[i].dir == MODE_ADD)
        mode_changes[i].letter = 0;

    mode_changes[mode_count].letter = c;
    mode_changes[mode_count].arg = lstr;
    mode_changes[mode_count].id = NULL;
    mode_changes[mode_count].dir = dir;
    mode_changes[mode_count++].mems = ALL_MEMBERS;

    chptr->mode.limit = limit;
  }
  else if (dir == MODE_DEL)
  {
    if (!chptr->mode.limit)
      return;

    chptr->mode.limit = 0;

    mode_changes[mode_count].letter = c;
    mode_changes[mode_count].arg = NULL;
    mode_changes[mode_count].id = NULL;
    mode_changes[mode_count].dir = dir;
    mode_changes[mode_count++].mems = ALL_MEMBERS;
  }
}
