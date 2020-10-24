idle_time_get(const struct Client *source_p, const struct Client *target_p)
{
  unsigned int idle = 0;
  unsigned int min_idle = 0;
  unsigned int max_idle = 0;
  const struct ClassItem *class = get_class_ptr(&target_p->localClient->confs);

  if (!(class->flags & CLASS_FLAGS_FAKE_IDLE) || target_p == source_p)
    return CurrentTime - target_p->localClient->last_privmsg;
  if (HasUMode(source_p, UMODE_OPER) &&
      !(class->flags & CLASS_FLAGS_HIDE_IDLE_FROM_OPERS))
    return CurrentTime - target_p->localClient->last_privmsg;

  min_idle = class->min_idle;
  max_idle = class->max_idle;

  if (min_idle == max_idle)
    return min_idle;

  if (class->flags & CLASS_FLAGS_RANDOM_IDLE)
    idle = genrand_int32();
  else
    idle = CurrentTime - target_p->localClient->last_privmsg;

  if (!max_idle)
    idle = 0;
  else
    idle %= max_idle;

  if (idle < min_idle)
    idle = min_idle + (idle % (max_idle - min_idle));

  return idle;
}
