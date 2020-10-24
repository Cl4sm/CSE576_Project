check_spambot_warning(struct Client *source_p, const char *name)
{
  int t_delta = 0;
  int decrement_count = 0;

  if ((GlobalSetOptions.spam_num &&
       (source_p->localClient->join_leave_count >=
        GlobalSetOptions.spam_num)))
  {
    if (source_p->localClient->oper_warn_count_down > 0)
      source_p->localClient->oper_warn_count_down--;
    else
      source_p->localClient->oper_warn_count_down = 0;

    if (source_p->localClient->oper_warn_count_down == 0)
    {
      /* It's already known as a possible spambot */
      if (name)
        sendto_realops_flags(UMODE_BOTS, L_ALL, SEND_NOTICE,
                             "User %s (%s@%s) trying to join %s is a possible spambot",
                             source_p->name, source_p->username,
                             source_p->host, name);
      else
        sendto_realops_flags(UMODE_BOTS, L_ALL, SEND_NOTICE,
                             "User %s (%s@%s) is a possible spambot",
                             source_p->name, source_p->username,
                             source_p->host);
      source_p->localClient->oper_warn_count_down = OPER_SPAM_COUNTDOWN;
    }
  }
  else
  {
    if ((t_delta = (CurrentTime - source_p->localClient->last_leave_time)) >
         JOIN_LEAVE_COUNT_EXPIRE_TIME)
    {
      decrement_count = (t_delta / JOIN_LEAVE_COUNT_EXPIRE_TIME);
      if (decrement_count > source_p->localClient->join_leave_count)
        source_p->localClient->join_leave_count = 0;
      else
        source_p->localClient->join_leave_count -= decrement_count;
    }
    else
    {
      if ((CurrentTime - (source_p->localClient->last_join_time)) <
          GlobalSetOptions.spam_time)
        source_p->localClient->join_leave_count++;  /* It's a possible spambot */
    }

    if (name)
      source_p->localClient->last_join_time = CurrentTime;
    else
      source_p->localClient->last_leave_time = CurrentTime;
  }
}
