static int
cmd_find_common_match(struct cmd_node *root, char *cmd, int len, int *pcount, char *buf)
{
  struct cmd_node *m;
  int best, best_prio, i;

  *pcount = 0;
  best = -1;
  best_prio = -1;
  for(m=root->son; m; m=m->sibling)
    {
      if (m->len < len || memcmp(m->token, cmd, len))
	continue;

      if (best_prio > m->prio)
	continue;

      if (best_prio < m->prio)
	{
	  *pcount = 0;
	  best = -1;
	}

      (*pcount)++;
      if (best < 0)
	{
	  strcpy(buf, m->token + len);
	  best = m->len - len;
	  best_prio = m->prio;
	}
      else
	{
	  i = 0;
	  while (i < best && i < m->len - len && buf[i] == m->token[len+i])
	    i++;
	  best = i;
	}
    }
  return best;
}
