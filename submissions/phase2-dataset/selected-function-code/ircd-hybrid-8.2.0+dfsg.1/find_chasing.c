struct Client *
find_chasing(struct Client *source_p, const char *name)
{
  struct Client *who = find_person(source_p, name);

  if (who)
    return who;

  if (IsDigit(*name))
    return NULL;

  if ((who = whowas_get_history(name,
                         (time_t)ConfigGeneral.kill_chase_time_limit))
                         == NULL)
  {
    sendto_one_numeric(source_p, &me, ERR_NOSUCHNICK, name);
    return NULL;
  }

  return who;
}
