chm_simple(struct Client *source_p, struct Channel *chptr, int parc, int *parn,
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

  /* If have already dealt with this simple mode, ignore it */
  if (simple_modes_mask & d)
    return;

  simple_modes_mask |= d;

  /* setting + */
  /* Apparently, (though no one has ever told the hybrid group directly)
   * admins don't like redundant mode checking. ok. It would have been nice
   * if you had have told us directly. I've left the original code snippets
   * in place.
   *
   * -Dianora
   */
  if (dir == MODE_ADD) /* && !(chptr->mode.mode & d)) */
  {
    chptr->mode.mode |= d;

    mode_changes[mode_count].letter = c;
    mode_changes[mode_count].arg = NULL;
    mode_changes[mode_count].id = NULL;
    mode_changes[mode_count].dir = dir;
    mode_changes[mode_count++].mems = ALL_MEMBERS;
  }
  else if (dir == MODE_DEL) /* && (chptr->mode.mode & d)) */
  {
    /* setting - */

    chptr->mode.mode &= ~d;

    mode_changes[mode_count].letter = c;
    mode_changes[mode_count].arg = NULL;
    mode_changes[mode_count].id = NULL;
    mode_changes[mode_count].dir = dir;
    mode_changes[mode_count++].mems = ALL_MEMBERS;
  }
}
