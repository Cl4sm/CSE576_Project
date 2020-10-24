valid_username(const char *username, const int local)
{
  unsigned int dots = 0;
  const char *p = username;

  assert(p);

  if (*p == '~')
    ++p;

  /*
   * Reject usernames that don't start with an alphanum
   * i.e. reject jokers who have '-@somehost' or '.@somehost'
   * or "-hi-@somehost", "h-----@somehost" would still be accepted.
   */
  if (!IsAlNum(*p))
    return 0;

  if (local)
  {
    while (*++p)
    {
      if (*p == '.' && ConfigGeneral.dots_in_ident)
      {
        if (++dots > ConfigGeneral.dots_in_ident)
          return 0;
        if (!IsUserChar(*(p + 1)))
          return 0;
      }
      else if (!IsUserChar(*p))
        return 0;
    }
  }
  else
  {
    while (*++p)
      if (!IsUserChar(*p))
        return 0;
  }

  return p - username <= USERLEN;
}
