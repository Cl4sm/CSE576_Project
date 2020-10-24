list_one_channel(struct Client *source_p, struct Channel *chptr)
{
  const struct ListTask *lt = source_p->localClient->list_task;
  char listbuf[MODEBUFLEN] = "";
  char modebuf[MODEBUFLEN] = "";
  char parabuf[MODEBUFLEN] = "";

  if (SecretChannel(chptr) &&
      !(HasUMode(source_p, UMODE_ADMIN) || IsMember(source_p, chptr)))
    return;
  if (dlink_list_length(&chptr->members) < lt->users_min ||
      dlink_list_length(&chptr->members) > lt->users_max ||
      (chptr->channelts != 0 &&
       ((unsigned int)chptr->channelts < lt->created_min ||
        (unsigned int)chptr->channelts > lt->created_max)) ||
      (unsigned int)chptr->topic_time < lt->topicts_min ||
      (chptr->topic_time ? (unsigned int)chptr->topic_time : UINT_MAX) >
      lt->topicts_max)
    return;

  if (lt->topic[0] && match(lt->topic, chptr->topic))
    return;

  if (!list_allow_channel(chptr->chname, lt))
    return;

  channel_modes(chptr, source_p, modebuf, parabuf);

  if (chptr->topic[0])
    snprintf(listbuf, sizeof(listbuf), "[%s] ", modebuf);
  else
    snprintf(listbuf, sizeof(listbuf), "[%s]",  modebuf);

  sendto_one_numeric(source_p, &me, RPL_LIST, chptr->chname,
                     dlink_list_length(&chptr->members),
                     listbuf, chptr->topic);
}
