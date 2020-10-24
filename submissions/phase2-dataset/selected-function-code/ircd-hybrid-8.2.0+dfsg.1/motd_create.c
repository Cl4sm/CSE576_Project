motd_create(const char *mask, const char *path)
{
  struct Motd *tmp = MyCalloc(sizeof(struct Motd));

  if (EmptyString(mask))
    tmp->type = MOTD_UNIVERSAL;
  else if (class_find(mask, 1))
    tmp->type = MOTD_CLASS;
  else
  {
    switch (parse_netmask(mask, &tmp->address, &tmp->addrbits))
    {
      case HM_IPV4:
      tmp->type = MOTD_IPMASKV4;
      break;
    case HM_IPV6:
      tmp->type = MOTD_IPMASKV6;
      break;
    default: /* HM_HOST */
      tmp->type = MOTD_HOSTMASK;
      break;
    }
  }

  if (mask)
    tmp->mask = xstrdup(mask);

  tmp->path = xstrdup(path);
  tmp->maxcount = MOTD_MAXLINES;

  return tmp;
}
