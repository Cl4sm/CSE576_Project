int
can_send(struct Channel *chptr, struct Client *source_p,
         struct Membership *ms, const char *message)
{
  struct MaskItem *conf = NULL;

  if (IsServer(source_p) || HasFlag(source_p, FLAGS_SERVICE))
    return CAN_SEND_OPV;

  if (MyClient(source_p) && !IsExemptResv(source_p))
    if (!(HasUMode(source_p, UMODE_OPER) && ConfigGeneral.oper_pass_resv))
      if ((conf = match_find_resv(chptr->chname)) && !resv_find_exempt(source_p, conf))
        return ERR_CANNOTSENDTOCHAN;

  if ((chptr->mode.mode & MODE_NOCTRL) && msg_has_ctrls(message))
    return ERR_NOCTRLSONCHAN;
  if (ms || (ms = find_channel_link(source_p, chptr)))
    if (ms->flags & (CHFL_CHANOP|CHFL_HALFOP|CHFL_VOICE))
      return CAN_SEND_OPV;
  if (!ms && (chptr->mode.mode & MODE_NOPRIVMSGS))
    return ERR_CANNOTSENDTOCHAN;
  if (chptr->mode.mode & MODE_MODERATED)
    return ERR_CANNOTSENDTOCHAN;
  if ((chptr->mode.mode & MODE_MODREG) && !HasUMode(source_p, UMODE_REGISTERED))
    return ERR_NEEDREGGEDNICK;

  /* Cache can send if banned */
  if (MyClient(source_p))
  {
    if (ms)
    {
      if (ms->flags & CHFL_BAN_SILENCED)
        return ERR_CANNOTSENDTOCHAN;

      if (!(ms->flags & CHFL_BAN_CHECKED))
      {
        if (is_banned(chptr, source_p))
        {
          ms->flags |= (CHFL_BAN_CHECKED|CHFL_BAN_SILENCED);
          return ERR_CANNOTSENDTOCHAN;
        }

        ms->flags |= CHFL_BAN_CHECKED;
      }
    }
    else if (is_banned(chptr, source_p))
      return ERR_CANNOTSENDTOCHAN;
  }

  return CAN_SEND_NONOP;
}
