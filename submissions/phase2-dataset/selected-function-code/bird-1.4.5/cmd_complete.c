int
cmd_complete(char *cmd, int len, char *buf, int again)
{
  char *start = cmd;
  char *end = cmd + len;
  char *fin;
  struct cmd_node *n, *m;
  char *z;
  int ambig, cnt = 0, common;

  /* Find the last word we want to complete */
  for(fin=end; fin > start && !isspace(fin[-1]); fin--)
    ;

  /* Find the context */
  n = &cmd_root;
  while (cmd < fin && n->son)
    {
      if (isspace(*cmd))
	{
	  cmd++;
	  continue;
	}
      z = cmd;
      while (cmd < fin && !isspace(*cmd))
	cmd++;
      m = cmd_find_abbrev(n, z, cmd-z, &ambig);
      if (ambig)
	{
	  if (!again)
	    return -1;
	  input_start_list();
	  cmd_list_ambiguous(n, z, cmd-z);
	  input_stop_list();
	  return 0;
	}
      if (!m)
	return -1;
      n = m;
    }

  /* Completion of parameters is not yet supported */
  if (!n->son)
    return -1;

  /* We know the context, let's try to complete */
  common = cmd_find_common_match(n, fin, end-fin, &cnt, buf);
  if (!cnt)
    return -1;
  if (cnt == 1)
    {
      buf[common++] = ' ';
      buf[common] = 0;
      return 1;
    }
  if (common > 0)
    {
      buf[common] = 0;
      return 1;
    }
  if (!again)
    return -1;
  input_start_list();
  cmd_list_ambiguous(n, fin, end-fin);
  input_stop_list();
  return 0;
}
