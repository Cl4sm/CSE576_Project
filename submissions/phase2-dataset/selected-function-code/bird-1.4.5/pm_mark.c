pm_mark(struct pm_pos *pos, int i, int plen, int *nl, int *nh)
{
  int j;

  if (pos[i].set)
    pos[i].mark = 1;
  
  for (j = i + 1; (j < plen) && pos[j].set && (! pos[j].mark); j++)
    pos[j].mark = 1;
  pos[j].mark = 1;

  /* We are going downwards, therefore every mark is
     new low and just the first mark is new high */

  *nl = i + (pos[i].set ? 0 : 1);

  if (*nh < 0)
    *nh = j;
}
