cmd_find_abbrev(struct cmd_node *root, char *cmd, int len, int *pambiguous)
{
  struct cmd_node *m, *best = NULL, *best2 = NULL;

  *pambiguous = 0;
  for(m=root->son; m; m=m->sibling)
    {
      if (m->len == len && !memcmp(m->token, cmd, len))
	return m;
      if (m->len > len && !memcmp(m->token, cmd, len))
	{
	  if (best && best->prio > m->prio)
	    continue;
	  if (best && best->prio == m->prio)
	    best2 = best;
	  best = m;
	}
    }
  if (best2)
    {
      *pambiguous = 1;
      return NULL;
    }
  return best;
}
