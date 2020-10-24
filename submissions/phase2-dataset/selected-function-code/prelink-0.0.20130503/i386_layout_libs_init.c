i386_layout_libs_init (struct layout_libs *l)
{
  if (exec_shield)
    {
      int i;
      struct prelink_entry *e;
      Elf32_Addr reg0s;

      if (l->max_page_size > 0x200000)
	error (EXIT_FAILURE, 0, "--layout-page-size too large");

      reg0s = (REG0S + l->max_page_size - 1) & ~(l->max_page_size - 1);
      l->mmap_base = reg0s;
      l->mmap_end = REG2E;
      /* Don't allow this to be overridden.  */
      mmap_reg_start = ~(GElf_Addr) 0;
      mmap_reg_end = ~(GElf_Addr) 0;
      for (i = 0; i < l->nlibs; ++i)
	{
	  e = l->libs[i];
	  if (e->done == 0)
	    continue;
	  if (e->base < reg0s
	      || (e->base < REG1S && e->layend > REG0E)
	      || (e->base < REG2S && e->layend > REG1E)
	      || e->layend > REG2E)
	    e->done = 0;
	}
    }
  else
    {
      l->mmap_base = REG2S;
      l->mmap_end = REG2E;
    }
  return 0;
}
