void
sendto_one_notice(struct Client *to, struct Client *from, const char *pattern, ...)
{
  struct dbuf_block *buffer = NULL;
  const char *dest = NULL;
  va_list args;

  if (IsDead(to->from))
    return;

  dest = ID_or_name(to, to);

  if (EmptyString(dest))
    dest = "*";

  buffer = dbuf_alloc();

  dbuf_put_fmt(buffer, ":%s NOTICE %s ", ID_or_name(from, to), dest);

  va_start(args, pattern);
  send_format(buffer, pattern, args);
  va_end(args);

  send_message(to->from, buffer);

  dbuf_ref_free(buffer);
}
