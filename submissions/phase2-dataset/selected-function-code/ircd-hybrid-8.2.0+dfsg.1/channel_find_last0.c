channel_find_last0(struct Client *source_p, char *chanlist)
{
  int join0 = 0;

  for (char *p = chanlist; *p; ++p)  /* Find last "JOIN 0" */
  {
    if (*p == '0' && (*(p + 1) == ',' || *(p + 1) == '\0'))
    {
      if ((*p + 1) == ',')
        ++p;

      chanlist = p + 1;
      join0 = 1;
    }
    else
    {
      while (*p != ',' && *p != '\0')  /* Skip past channel name */
        ++p;

      if (*p == '\0')  /* Hit the end */
        break;
    }
  }

  if (join0)
    channel_do_join_0(source_p);

  return chanlist;
}
