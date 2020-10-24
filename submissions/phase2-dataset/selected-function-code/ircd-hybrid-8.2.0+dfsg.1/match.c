int
match(const char *mask, const char *name)
{
  const char *m = mask, *n = name;
  const char *m_tmp = mask, *n_tmp = name;
  unsigned int star = 0;

  while (1)
  {
    switch (*m)
    {
      case '\0':
        if (!*n)
          return 0;
  backtrack:
        if (m_tmp == mask)
          return 1;
        m = m_tmp;
        n = ++n_tmp;
        if (*n == '\0')
          return 1;
        break;
      case '\\':
        ++m;
        /* allow escaping to force capitalization */
        if (*m++ != *n++)
          goto backtrack;
        break;
      case '*':
      case '?':
        for (star = 0; ; ++m)
        {
          if (*m == '*')
            star = 1;
          else if (*m == '?')
          {
            if (!*n++)
              goto backtrack;
          }
          else
            break;
        }

        if (star)
        {
          if (!*m)
            return 0;
          else if (*m == '\\')
          {
            m_tmp = ++m;
            if (!*m)
              return 1;
            for (n_tmp = n; *n && *n != *m; ++n)
              ;
          }
          else
          {
            m_tmp = m;
            for (n_tmp = n; *n && (ToLower(*n) != ToLower(*m)); ++n)
              ;
          }
        }
        /* and fall through */
      default:
        if (!*n)
          return *m != '\0';
        if (ToLower(*m) != ToLower(*n))
          goto backtrack;
        ++m;
        ++n;
        break;
    }
  }

  return 1;
}
