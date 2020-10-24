static int
i386_layout_libs_pre (struct layout_libs *l)
{
  Elf32_Addr mmap_start, virt, reg0s;
  struct prelink_entry *e, *next;
  struct i386_layout_data *pld;
  int i;

  if (!exec_shield)
    {
      l->mmap_fin = l->mmap_end;
      l->fake = NULL;
      l->fakecnt = 0;
      return 0;
    }

  pld = calloc (sizeof (*pld), 1);
  if (pld == NULL)
    error (EXIT_FAILURE, ENOMEM, "Cannot lay libraries out");

  l->arch_data = pld;
  reg0s = (REG0S + l->max_page_size - 1) & ~(l->max_page_size - 1);

  mmap_start = l->mmap_start - reg0s;
  /* Unless not randomizing, try not to make the first region
     too small, because otherwise it is likely libc.so as first
     big library would often end up at reg0s.  */
  virt = mmap_start % (REG0E - reg0s - 0x200000);
  i386_find_free_addr (l, pld->addrs + 0, reg0s, REG0E, reg0s + virt);
  virt = mmap_start % (REG1E - REG1S - 0x200000);
  i386_find_free_addr (l, pld->addrs + 4, REG1S, REG1E, REG1S + virt);
  virt = mmap_start % (REG2E - REG2S - 0x200000);
  i386_find_free_addr (l, pld->addrs + 8, REG2S, REG2E, REG2S + virt);
  i = 0;
  virt = pld->addrs[3] - pld->addrs[2];
  pld->e[0].u.tmp = -1;
  pld->e[0].base = virt;
  pld->e[0].end = pld->e[0].base;
  pld->e[0].layend = pld->e[0].end;
  pld->e[0].prev = &pld->e[0];
  next = NULL;
  for (e = l->list; e != NULL; e = next)
    {
      next = e->next;
      while (i < 5
	     && (e->base >= pld->addrs[2 * i + 1]
		 || pld->addrs[2 * i] == pld->addrs[2 * i + 1]))
	{
	  ++i;
	  pld->e[i].u.tmp = -1;
	  if (i & 1)
	    virt -= pld->addrs[2 * i + 1] - pld->addrs[2 * i];
	  else
	    {
	      virt += pld->addrs[2 * i - 1] - pld->addrs[2 * i - 4];
	      virt += pld->addrs[2 * i + 3] - pld->addrs[2 * i + 2];
	    }
	  pld->e[i].base = virt;
	  pld->e[i].end = pld->e[i].base;
	  pld->e[i].layend = pld->e[i].end;
	  pld->e[i].prev = &pld->e[i];
	}
      e->base += (Elf32_Sword) (virt - pld->addrs[2 * i]);
      e->end += (Elf32_Sword) (virt - pld->addrs[2 * i]);
      e->layend += (Elf32_Sword) (virt - pld->addrs[2 * i]);
      list_append (&pld->e[i], e);
    }
  while (i < 5)
    {
      ++i;
      pld->e[i].u.tmp = -1;
      if (i & 1)
	virt -= pld->addrs[2 * i + 1] - pld->addrs[2 * i];
      else
	{
	  virt += pld->addrs[2 * i - 1] - pld->addrs[2 * i - 4];
	  virt += pld->addrs[2 * i + 3] - pld->addrs[2 * i + 2];
	}
      pld->e[i].base = virt;
      pld->e[i].end = pld->e[i].base;
      pld->e[i].layend = pld->e[i].end;
      pld->e[i].prev = &pld->e[i];
    }
  l->list = &pld->e[1];
  list_merge (&pld->e[1], &pld->e[0]);
  list_merge (&pld->e[1], &pld->e[3]);
  list_merge (&pld->e[1], &pld->e[2]);
  list_merge (&pld->e[1], &pld->e[5]);
  list_merge (&pld->e[1], &pld->e[4]);

  l->mmap_start = 0;
  l->mmap_base = 0;
  l->mmap_fin = virt + pld->addrs[2 * i + 1] - pld->addrs[2 * i];
  l->mmap_end = l->mmap_fin;
  l->fakecnt = 6;
  l->fake = pld->e;

  return 0;
}
