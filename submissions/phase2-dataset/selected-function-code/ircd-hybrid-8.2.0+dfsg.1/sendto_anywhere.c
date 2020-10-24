sendto_anywhere(struct Client *to, struct Client *from,
                const char *command,
                const char *pattern, ...)
{
  va_list args;
  struct dbuf_block *buffer = NULL;

  if (IsDead(to->from))
    return;

  buffer = dbuf_alloc();

  if (MyClient(to) && IsClient(from))
    dbuf_put_fmt(buffer, ":%s!%s@%s %s %s ", from->name, from->username,
                 from->host, command, to->name);
  else
    dbuf_put_fmt(buffer, ":%s %s %s ", ID_or_name(from, to),
                 command, ID_or_name(to, to));

  va_start(args, pattern);
  send_format(buffer, pattern, args);
  va_end(args);

  if (MyClient(to))
    send_message(to, buffer);
  else
    send_message_remote(to, from, buffer);

  dbuf_ref_free(buffer);
}
