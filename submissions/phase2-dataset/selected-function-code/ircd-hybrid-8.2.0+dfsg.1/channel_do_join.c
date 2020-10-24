channel_do_join(struct Client *source_p, char *channel, char *key_list)
{
  char *p = NULL;
  char *chan = NULL;
  char *chan_list = NULL;
  struct Channel *chptr = NULL;
  struct MaskItem *conf = NULL;
  const struct ClassItem *class = get_class_ptr(&source_p->localClient->confs);
  int i = 0;
  unsigned int flags = 0;

  chan_list = channel_find_last0(source_p, channel);

  for (chan = strtoken(&p, chan_list, ","); chan;
       chan = strtoken(&p,      NULL, ","))
  {
    const char *key = NULL;

    /* If we have any more keys, take the first for this channel. */
    if (!EmptyString(key_list) && (key_list = strchr(key = key_list, ',')))
      *key_list++ = '\0';

    /* Empty keys are the same as no keys. */
    if (key && *key == '\0')
      key = NULL;

    if (!check_channel_name(chan, 1))
    {
      sendto_one_numeric(source_p, &me, ERR_BADCHANNAME, chan);
      continue;
    }

    if (!IsExemptResv(source_p) &&
        !(HasUMode(source_p, UMODE_OPER) && ConfigGeneral.oper_pass_resv) &&
        ((conf = match_find_resv(chan)) && !resv_find_exempt(source_p, conf)))
    {
      ++conf->count;
      sendto_one_numeric(source_p, &me, ERR_CHANBANREASON,
                         chan, conf->reason ? conf->reason : "Reserved channel");
      sendto_realops_flags(UMODE_REJ, L_ALL, SEND_NOTICE,
                           "Forbidding reserved channel %s from user %s",
                           chan, get_client_name(source_p, HIDE_IP));
      continue;
    }

    if (dlink_list_length(&source_p->channel) >=
        ((class->max_channels) ? class->max_channels : ConfigChannel.max_channels))
    {
      sendto_one_numeric(source_p, &me, ERR_TOOMANYCHANNELS, chan);
      break;
    }

    if ((chptr = hash_find_channel(chan)))
    {
      if (IsMember(source_p, chptr))
        continue;

      if (splitmode && !HasUMode(source_p, UMODE_OPER) &&
          ConfigChannel.no_join_on_split)
      {
        sendto_one_numeric(source_p, &me, ERR_UNAVAILRESOURCE, chptr->chname);
        continue;
      }

      /*
       * can_join checks for +i key, bans.
       */
      if ((i = can_join(source_p, chptr, key)))
      {
        sendto_one_numeric(source_p, &me, i, chptr->chname);
        continue;
      }

      /*
       * This should never be the case unless there is some sort of
       * persistant channels.
       */
      if (dlink_list_length(&chptr->members) == 0)
        flags = CHFL_CHANOP;
      else
        flags = 0;
    }
    else
    {
      if (splitmode && !HasUMode(source_p, UMODE_OPER) &&
          (ConfigChannel.no_create_on_split || ConfigChannel.no_join_on_split))
      {
        sendto_one_numeric(source_p, &me, ERR_UNAVAILRESOURCE, chan);
        continue;
      }

      flags = CHFL_CHANOP;
      chptr = make_channel(chan);
    }

    if (!HasUMode(source_p, UMODE_OPER))
      check_spambot_warning(source_p, chptr->chname);

    add_user_to_channel(chptr, source_p, flags, 1);

    /*
     *  Set timestamp if appropriate, and propagate
     */
    if (flags == CHFL_CHANOP)
    {
      chptr->channelts = CurrentTime;
      chptr->mode.mode |= MODE_TOPICLIMIT;
      chptr->mode.mode |= MODE_NOPRIVMSGS;

      sendto_server(source_p, NOCAPS, NOCAPS, ":%s SJOIN %lu %s +nt :@%s",
                    me.id, (unsigned long)chptr->channelts,
                    chptr->chname, source_p->id);

      /*
       * Notify all other users on the new channel
       */
      sendto_channel_local(ALL_MEMBERS, 0, chptr, ":%s!%s@%s JOIN :%s",
                           source_p->name, source_p->username,
                           source_p->host, chptr->chname);
      sendto_channel_local(ALL_MEMBERS, 0, chptr, ":%s MODE %s +nt",
                           me.name, chptr->chname);

      if (source_p->away[0])
        sendto_channel_local_butone(source_p, 0, CAP_AWAY_NOTIFY, chptr,
                                    ":%s!%s@%s AWAY :%s",
                                    source_p->name, source_p->username,
                                    source_p->host, source_p->away);
    }
    else
    {
      sendto_server(source_p, NOCAPS, NOCAPS, ":%s JOIN %lu %s +",
                    source_p->id, (unsigned long)chptr->channelts,
                    chptr->chname);
      sendto_channel_local(ALL_MEMBERS, 0, chptr, ":%s!%s@%s JOIN :%s",
                           source_p->name, source_p->username,
                           source_p->host, chptr->chname);

      if (source_p->away[0])
        sendto_channel_local_butone(source_p, 0, CAP_AWAY_NOTIFY, chptr,
                                    ":%s!%s@%s AWAY :%s",
                                    source_p->name, source_p->username,
                                    source_p->host, source_p->away);
    }

    del_invite(chptr, source_p);

    if (chptr->topic[0])
    {
      sendto_one_numeric(source_p, &me, RPL_TOPIC, chptr->chname, chptr->topic);
      sendto_one_numeric(source_p, &me, RPL_TOPICWHOTIME, chptr->chname,
                         chptr->topic_info, chptr->topic_time);
    }

    channel_member_names(source_p, chptr, 1);

    source_p->localClient->last_join_time = CurrentTime;
  }
}
