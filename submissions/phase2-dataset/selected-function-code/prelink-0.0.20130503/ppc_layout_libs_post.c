static int
ppc_layout_libs_post (struct layout_libs *l)
{
  struct prelink_entry *e;
  struct ppc_layout_data *pld = (struct ppc_layout_data *) l->arch_data;
  Elf32_Addr base, end;
  int i;

  /* First fix up base and end fields we saved.  */
  for (i = 0; i < pld->cnt; ++i)
    {
      pld->ents[i].e->base = pld->ents[i].base;
      pld->ents[i].e->layend = pld->ents[i].layend;
      pld->ents[i].e->end = pld->ents[i].end;
      pld->ents[i].e->done |= 0x40;
    }
  pld->e[0].done |= 0x40;
  pld->e[1].done |= 0x40;
  pld->e[2].done |= 0x40;

  /* Now fix up the newly created items.  */
  for (e = l->list; e != NULL; e = e->next)
    if (e->done & 0x40)
      e->done &= ~0x40;
    else
      {
	base = e->base;
	end = e->layend;
	if (e->base < pld->e[0].base)
	  {
	    e->base = REG1S + REG0E - end;
	    e->end += e->base - base;
	    e->layend = REG1S + REG0E - base;
	  }
	else if (e->base < pld->e[1].base)
	  {
	    e->base = pld->e[0].end + pld->mmap_start - end;
	    e->end += e->base - base;
	    e->layend = pld->e[0].end + pld->mmap_start - base;
	  }
	else if (e->base < pld->e[2].base)
	  {
	    e->base -= pld->e[1].end - REG1S;
	    e->end -= pld->e[1].end - REG1S;
	    e->layend -= pld->e[1].end - REG1S;
	  }
	else
	  {
	    e->base -= pld->e[2].end - pld->last_start;
	    e->end -= pld->e[2].end - pld->last_start;
	    e->layend -= pld->e[2].end - pld->last_start;
	  }
      }

  for (i = 0; i < pld->cnt; ++i)
    pld->ents[i].e->done &= ~0x40;

  free (l->arch_data);
  return 0;
}
