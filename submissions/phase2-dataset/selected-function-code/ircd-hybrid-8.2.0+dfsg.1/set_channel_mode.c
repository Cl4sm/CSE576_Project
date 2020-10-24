set_channel_mode(struct Client *source_p, struct Channel *chptr,
                 struct Membership *member, int parc, char *parv[])
{
  int dir = MODE_ADD;
  int parn = 1;
  int alevel = 0, errors = 0;

  mode_count = 0;
  mode_limit = 0;
  simple_modes_mask = 0;

  alevel = get_channel_access(source_p, member);

  for (const char *ml = parv[0]; *ml; ++ml)
  {
    switch (*ml)
    {
      case '+':
        dir = MODE_ADD;
        break;
      case '-':
        dir = MODE_DEL;
        break;
      case '=':
        dir = MODE_QUERY;
        break;
      default:
      {
        const struct ChannelMode *tptr = &ModeTable[(unsigned char)*ml];

        tptr->func(source_p, chptr, parc, &parn, parv,
                   &errors, alevel, dir, *ml, tptr->d);
        break;
      }
    }
  }

  send_mode_changes(source_p, chptr);
}
