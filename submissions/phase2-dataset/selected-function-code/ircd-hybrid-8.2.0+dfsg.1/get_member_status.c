const char *
get_member_status(const struct Membership *ms, const int combine)
{
  static char buffer[4];  /* 4 for @%+\0 */
  char *p = buffer;

  if (ms->flags & CHFL_CHANOP)
  {
    if (!combine)
      return "@";
    *p++ = '@';
  }

  if (ms->flags & CHFL_HALFOP)
  {
    if (!combine)
      return "%";
    *p++ = '%';
  }

  if (ms->flags & CHFL_VOICE)
    *p++ = '+';
  *p = '\0';

  return buffer;
}
