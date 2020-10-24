whowas_get_history(const char *nick, time_t timelimit)
{
  dlink_node *ptr = NULL;

  timelimit = CurrentTime - timelimit;

  DLINK_FOREACH(ptr, WHOWASHASH[strhash(nick)].head)
  {
    struct Whowas *temp = ptr->data;

    if (temp->logoff < timelimit)
      continue;
    if (irccmp(nick, temp->name))
      continue;
    return temp->online;
  }

  return NULL;
}
