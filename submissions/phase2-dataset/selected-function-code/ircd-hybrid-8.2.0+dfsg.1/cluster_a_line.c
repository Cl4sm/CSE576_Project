cluster_a_line(struct Client *source_p, const char *command,
               int capab, int cluster_type, const char *pattern, ...)
{
  va_list args;
  char buffer[IRCD_BUFSIZE] = "";
  const dlink_node *ptr = NULL;

  va_start(args, pattern);
  vsnprintf(buffer, sizeof(buffer), pattern, args);
  va_end(args);

  DLINK_FOREACH(ptr, cluster_items.head)
  {
    const struct MaskItem *conf = ptr->data;

    if (conf->flags & cluster_type)
      sendto_match_servs(source_p, conf->name, CAP_CLUSTER|capab,
                         "%s %s %s", command, conf->name, buffer);
  }
}
