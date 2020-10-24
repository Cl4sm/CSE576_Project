chm_except(struct Client *source_p, struct Channel *chptr, int parc, int *parn,
           char **parv, int *errors, int alev, int dir, char c, unsigned int d)
{
  char *mask = NULL;

  if (dir == MODE_QUERY || parc <= *parn)
  {
    dlink_node *ptr = NULL;

    if (*errors & SM_ERR_RPL_E)
      return;

    *errors |= SM_ERR_RPL_E;

    DLINK_FOREACH(ptr, chptr->exceptlist.head)
    {
      const struct Ban *banptr = ptr->data;

      sendto_one_numeric(source_p, &me, RPL_EXCEPTLIST, chptr->chname,
                         banptr->name, banptr->user, banptr->host,
                         banptr->who, banptr->when);
    }

    sendto_one_numeric(source_p, &me, RPL_ENDOFEXCEPTLIST, chptr->chname);
    return;
  }

  if (alev < CHACCESS_HALFOP)
  {
    if (!(*errors & SM_ERR_NOOPS))
      sendto_one_numeric(source_p, &me,
                         alev == CHACCESS_NOTONCHAN ? ERR_NOTONCHANNEL :
                         ERR_CHANOPRIVSNEEDED, chptr->chname);
    *errors |= SM_ERR_NOOPS;
    return;
  }

  if (MyClient(source_p) && (++mode_limit > MAXMODEPARAMS))
    return;

  mask = nuh_mask[*parn];
  memcpy(mask, parv[*parn], sizeof(nuh_mask[*parn]));
  ++*parn;

  if (!MyConnect(source_p))
    if (strchr(mask, ' '))
      return;

  switch (dir)
  {
    case MODE_ADD:
      if (!add_id(source_p, chptr, mask, CHFL_EXCEPTION))
        return;
      break;
    case MODE_DEL:
      if (!del_id(chptr, mask, CHFL_EXCEPTION))
        return;
      break;
    default:
      assert(0);
  }

  mode_changes[mode_count].letter = c;
  mode_changes[mode_count].arg = mask;
  mode_changes[mode_count].id = NULL;
  mode_changes[mode_count].dir = dir;
  mode_changes[mode_count++].mems = ALL_MEMBERS;
}
