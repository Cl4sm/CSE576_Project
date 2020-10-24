void
cmd_help(char *cmd, int len)
{
  char *end = cmd + len;
  struct cmd_node *n, *m;
  char *z;
  int ambig;

  n = &cmd_root;
  while (cmd < end)
    {
      if (isspace(*cmd))
	{
	  cmd++;
	  continue;
	}
      z = cmd;
      while (cmd < end && !isspace(*cmd))
	cmd++;
      m = cmd_find_abbrev(n, z, cmd-z, &ambig);
      if (ambig)
	{
	  cmd_list_ambiguous(n, z, cmd-z);
	  return;
	}
      if (!m)
	break;
      n = m;
    }
  cmd_display_help(n->cmd, NULL);
  for (m=n->son; m; m=m->sibling)
    cmd_display_help(m->help, m->cmd);
}
