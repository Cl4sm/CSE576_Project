sendto_one_numeric(struct Client *to, struct Client *from, enum irc_numerics numeric, ...)
{
  struct dbuf_block *buffer = NULL;
  const char *dest = NULL, *numstr = NULL;
  va_list args;

  if (IsDead(to->from))
    return;

  dest = ID_or_name(to, to);

  if (EmptyString(dest))
    dest = "*";

  buffer = dbuf_alloc();

  dbuf_put_fmt(buffer, ":%s %03d %s ", ID_or_name(from, to), numeric & ~SND_EXPLICIT, dest);

  va_start(args, numeric);

  if (numeric & SND_EXPLICIT)
    numstr = va_arg(args, const char *);
  else
    numstr = numeric_form(numeric);
  send_format(buffer, numstr, args);
  va_end(args);

  send_message(to->from, buffer);

  dbuf_ref_free(buffer);
}
