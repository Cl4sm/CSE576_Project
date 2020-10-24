char *
cmd_expand(char *cmd)
{
  struct cmd_node *n, *m;
  char *c, *b, *args;
  int ambig;

  args = c = cmd;
  n = &cmd_root;
  while (*c)
    {
      if (isspace(*c))
	{
	  c++;
	  continue;
	}
      b = c;
      while (*c && !isspace(*c))
	c++;
      m = cmd_find_abbrev(n, b, c-b, &ambig);
      if (!m)
	{
	  if (!ambig)
	    break;
	  puts("Ambiguous command, possible expansions are:");
	  cmd_list_ambiguous(n, b, c-b);
	  return NULL;
	}
      args = c;
      n = m;
    }
  if (!n->cmd)
    {
      puts("No such command. Press `?' for help.");
      return NULL;
    }
  b = xmalloc(strlen(n->cmd->command) + strlen(args) + 1);
  sprintf(b, "%s%s", n->cmd->command, args);
  return b;
}
