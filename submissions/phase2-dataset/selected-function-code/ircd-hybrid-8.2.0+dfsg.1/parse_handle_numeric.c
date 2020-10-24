static void
parse_handle_numeric(unsigned int numeric, struct Client *source_p, int parc, char *parv[])
{
  struct Client *target_p = NULL;
  struct Channel *chptr = NULL;

  /*
   * Avoid trash, we need it to come from a server and have a target
   */
  if (parc < 2 || !IsServer(source_p))
    return;

  /*
   * Who should receive this message ? Will we do something with it ?
   * Note that we use findUser functions, so the target can't be neither
   * a server, nor a channel (?) nor a list of targets (?) .. u2.10
   * should never generate numeric replies to non-users anyway
   * Ahem... it can be a channel actually, csc bots use it :\ --Nem
   */
  if (IsChanPrefix(*parv[1]))
    chptr = hash_find_channel(parv[1]);
  else
    target_p = find_person(source_p, parv[1]);

  if (((!target_p) || (target_p->from == source_p->from)) && !chptr)
    return;

  /*
   * Remap low number numerics, not that I understand WHY.. --Nemesi
   */
  /*
   * Numerics below 100 talk about the current 'connection', you're not
   * connected to a remote server so it doesn't make sense to send them
   * remotely - but the information they contain may be useful, so we
   * remap them up. Weird, but true.  -- Isomer
   */
  if (numeric < 100)
    numeric += 100;

  if (target_p)
  {
    /* Fake it for server hiding, if it's our client */
    if ((ConfigServerHide.hide_servers || IsHidden(source_p)) && MyConnect(target_p) &&
        !HasUMode(target_p, UMODE_OPER))
      sendto_one_numeric(target_p, &me, numeric|SND_EXPLICIT, "%s", parv[2]);
    else
      sendto_one_numeric(target_p, source_p, numeric|SND_EXPLICIT, "%s", parv[2]);
  }
  else
    sendto_channel_butone(source_p, source_p, chptr, 0, "%u %s %s",
                          numeric, chptr->chname, parv[2]);
}
