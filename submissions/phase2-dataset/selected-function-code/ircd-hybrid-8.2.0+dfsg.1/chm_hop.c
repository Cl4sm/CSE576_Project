static void
chm_hop(struct Client *source_p, struct Channel *chptr, int parc, int *parn,
        char **parv, int *errors, int alev, int dir, char c, unsigned int d)
{
  struct Client *target_p;
  struct Membership *member;

  if (alev < CHACCESS_CHANOP)
  {
    if (!(*errors & SM_ERR_NOOPS))
      sendto_one_numeric(source_p, &me,
                         alev == CHACCESS_NOTONCHAN ? ERR_NOTONCHANNEL :
                         ERR_CHANOPRIVSNEEDED, chptr->chname);
    *errors |= SM_ERR_NOOPS;
    return;
  }

  if (dir == MODE_QUERY || parc <= *parn)
    return;

  if ((target_p = find_chasing(source_p, parv[(*parn)++])) == NULL)
    return;  /* find_chasing sends ERR_NOSUCHNICK */

  if ((member = find_channel_link(target_p, chptr)) == NULL)
  {
    if (!(*errors & SM_ERR_NOTONCHANNEL))
      sendto_one_numeric(source_p, &me, ERR_USERNOTINCHANNEL, target_p->name, chptr->chname);
    *errors |= SM_ERR_NOTONCHANNEL;
    return;
  }

  if (MyClient(source_p) && (++mode_limit > MAXMODEPARAMS))
    return;

  /* no redundant mode changes */
  if (dir == MODE_ADD &&  has_member_flags(member, CHFL_HALFOP))
    return;
  if (dir == MODE_DEL && !has_member_flags(member, CHFL_HALFOP))
    return;

  mode_changes[mode_count].letter = c;
  mode_changes[mode_count].arg = target_p->name;
  mode_changes[mode_count].id = target_p->id;
  mode_changes[mode_count].dir = dir;
  mode_changes[mode_count++].mems = ALL_MEMBERS;

  if (dir == MODE_ADD)
    AddMemberFlag(member, CHFL_HALFOP);
  else
    DelMemberFlag(member, CHFL_HALFOP);
}
