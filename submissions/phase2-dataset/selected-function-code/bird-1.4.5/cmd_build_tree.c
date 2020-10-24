cmd_build_tree(void)
{
  unsigned int i;

  cmd_root.plastson = &cmd_root.son;

  for(i=0; i<ARRAY_SIZE(command_table); i++)
    {
      struct cmd_info *cmd = &command_table[i];
      struct cmd_node *old, *new;
      char *c = cmd->command;

      old = &cmd_root;
      while (*c)
	{
	  char *d = c;
	  while (*c && !isspace(*c))
	    c++;
	  for(new=old->son; new; new=new->sibling)
	    if (new->len == c-d && !memcmp(new->token, d, c-d))
	      break;
	  if (!new)
	    {
	      int size = sizeof(struct cmd_node) + c-d;
	      new = xmalloc(size);
	      bzero(new, size);
	      *old->plastson = new;
	      old->plastson = &new->sibling;
	      new->plastson = &new->son;
	      new->len = c-d;
	      memcpy(new->token, d, c-d);
	      new->prio = (new->len == 3 && !memcmp(new->token, "roa", 3)) ? 0 : 1; /* Hack */
	    }
	  old = new;
	  while (isspace(*c))
	    c++;
	}
      if (cmd->is_real_cmd)
	old->cmd = cmd;
      else
	old->help = cmd;
    }
}
