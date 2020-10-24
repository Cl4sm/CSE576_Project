static int
i386_layout_libs_post (struct layout_libs *l)
{
  struct prelink_entry *e;
  struct i386_layout_data *pld = (struct i386_layout_data *) l->arch_data;
  Elf32_Sword adj = 0;
  int i;

  if (!exec_shield)
    return 0;

  for (i = 0, e = l->list; e != NULL; e = e->next)
    {
      if (e == &pld->e[i ^ 1])
	{
	  adj = pld->addrs[2 * (i ^ 1)] - e->base;
	  ++i;
	}
      else
	{
	  e->base += adj;
	  e->end += adj;
	  e->layend += adj;
	}
    }

  free (l->arch_data);
  return 0;
}
