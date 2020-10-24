valid_wild_card(struct Client *source_p, int warn, int count, ...)
{
  unsigned char tmpch = '\0';
  unsigned int nonwild = 0;
  va_list args;

  /*
   * Now we must check the user and host to make sure there
   * are at least NONWILDCHARS non-wildcard characters in
   * them, otherwise assume they are attempting to kline
   * *@* or some variant of that. This code will also catch
   * people attempting to kline *@*.tld, as long as NONWILDCHARS
   * is greater than 3. In that case, there are only 3 non-wild
   * characters (tld), so if NONWILDCHARS is 4, the kline will
   * be disallowed.
   * -wnder
   */

  va_start(args, count);

  while (count--)
  {
    const unsigned char *p = va_arg(args, const unsigned char *);
    if (p == NULL)
      continue;

    while ((tmpch = *p++))
    {
      if (!IsKWildChar(tmpch))
      {
        /*
         * If we find enough non-wild characters, we can
         * break - no point in searching further.
         */
        if (++nonwild >= ConfigGeneral.min_nonwildcard)
        {
          va_end(args);
          return 1;
        }
      }
    }
  }

  if (warn)
    sendto_one_notice(source_p, &me,
                      ":Please include at least %u non-wildcard characters with the mask",
                      ConfigGeneral.min_nonwildcard);
  va_end(args);
  return 0;
}
