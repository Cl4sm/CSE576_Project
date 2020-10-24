int
find_readonly_space (DSO *dso, GElf_Shdr *add, GElf_Ehdr *ehdr,
		     GElf_Phdr *phdr, GElf_Shdr *shdr,
		     struct readonly_adjust *adjust)
{
  int i, j;
  GElf_Addr addr;
  GElf_Off p_filesz;

  if (add->sh_addr)
    {
      /* Prefer the current address if possible.  */
      for (i = 0; i < ehdr->e_phnum; ++i)
	if (phdr[i].p_type == PT_LOAD
	    && (phdr[i].p_flags & (PF_R | PF_W)) == PF_R
	    && phdr[i].p_vaddr <= add->sh_addr
	    && phdr[i].p_vaddr + phdr[i].p_filesz
	       >= add->sh_addr + add->sh_size)
	  break;

      if (i < ehdr->e_phnum)
	for (j = 1; j < ehdr->e_shnum; ++j)
	  if ((shdr[j].sh_flags & SHF_ALLOC)
	      && shdr[j].sh_addr >= add->sh_addr)
	    {
	      if (shdr[j].sh_addr >= add->sh_addr + add->sh_size
		  && shdr[j - 1].sh_addr + shdr[j - 1].sh_size <= add->sh_addr)
		{
		  insert_readonly_section (ehdr, shdr, j, adjust);
		  shdr[j] = *add;
		  shdr[j].sh_offset = (shdr[j].sh_addr - phdr[i].p_vaddr)
				       + phdr[i].p_offset;
		  return j;
		}
	      break;
	    }
    }

  for (i = 0; i < ehdr->e_phnum; ++i)
    if (phdr[i].p_type == PT_LOAD
	&& (phdr[i].p_flags & (PF_R | PF_W)) == PF_R)
      {
	GElf_Addr start = phdr[i].p_vaddr;
	int after = -1, min;

	if (phdr[i].p_offset < ehdr->e_phoff)
	  {
	    for (j = 0; j < ehdr->e_phnum; j++)
	      if (phdr[j].p_type == PT_PHDR
		  && phdr[j].p_offset == ehdr->e_phoff
		  && phdr[j].p_filesz >= ehdr->e_phnum * ehdr->e_phentsize)
		break;
	    start += ehdr->e_phoff;
	    if (j < ehdr->e_phnum)
	      start += phdr[j].p_filesz;
	    else
	      start += ehdr->e_phnum * ehdr->e_phentsize;
	    start -= phdr[i].p_offset;
	  }
	start = (start + add->sh_addralign - 1) & ~(add->sh_addralign - 1);
	for (j = 1; j < ehdr->e_shnum; ++j)
	  if ((shdr[j].sh_flags & SHF_ALLOC)
	      && shdr[j].sh_addr >= phdr[i].p_vaddr
	      && shdr[j].sh_addr + shdr[j].sh_size
		 <= phdr[i].p_vaddr + phdr[i].p_filesz)
	    {
	      if (after == -1)
		after = j - 1;
	      if (start + add->sh_size > shdr[j].sh_addr)
		{
		  start = shdr[j].sh_addr + shdr[j].sh_size;
		  start = (start + add->sh_addralign - 1)
			  & ~(add->sh_addralign - 1);
		  after = j;
		}
	    }

	min = -1;
	for (j = i + 1; j < ehdr->e_phnum; ++j)
	  if (phdr[j].p_offset >= phdr[i].p_offset + phdr[i].p_filesz
	      && (min == -1 || phdr[min].p_offset > phdr[j].p_offset))
	    min = j;

	if (after != -1
	    && (start + add->sh_size <= phdr[i].p_vaddr + phdr[i].p_filesz
		|| (phdr[i].p_filesz == phdr[i].p_memsz
		    && (min == -1
			|| start + add->sh_size - phdr[i].p_vaddr
			   <= phdr[min].p_offset))))
	  {
	    insert_readonly_section (ehdr, shdr, after + 1, adjust);
	    shdr[after + 1] = *add;
	    shdr[after + 1].sh_addr = start;
	    shdr[after + 1].sh_offset = (start - phdr[i].p_vaddr)
					 + phdr[i].p_offset;
	    if (start + add->sh_size > phdr[i].p_vaddr + phdr[i].p_filesz)
	      {
		adjust_nonalloc (dso, ehdr, shdr, 0, 0,
				 start + add->sh_size - phdr[i].p_vaddr
				 - phdr[i].p_filesz);
		phdr[i].p_filesz = start + add->sh_size - phdr[i].p_vaddr;
		phdr[i].p_memsz = phdr[i].p_filesz;
	      }
	    return after + 1;
	  }
      }

  /* If SHT_NOBITS sections are small, just extend the last PT_LOAD
     segment.  Small enough here means that the whole .bss fits into
     the same CPU page as the alloced part of it.  */
  for (i = -1, j = 0; j < ehdr->e_phnum; ++j)
    if (phdr[j].p_type == PT_LOAD)
      i = j;
  p_filesz = phdr[i].p_filesz;

  /* If we'll be converting NOBITS .plt to PROGBITS, account for that in the
     calculation.  */
  for (j = 1; j < ehdr->e_shnum; ++j)
    {
      if (shdr[j].sh_type == SHT_NOBITS
	  && shdr[j].sh_addr >= phdr[i].p_vaddr
	  && shdr[j].sh_addr + shdr[j].sh_size
	     <= phdr[i].p_vaddr + phdr[i].p_memsz
	  && !strcmp (strptr (dso, ehdr->e_shstrndx, shdr[j].sh_name), ".plt"))
	{
	  if (shdr[j].sh_addr + shdr[j].sh_size - phdr[i].p_vaddr > p_filesz)
	    p_filesz = shdr[j].sh_addr + shdr[j].sh_size - phdr[i].p_vaddr;
	  break;
	}
    }

  if (phdr[i].p_filesz
      && p_filesz <= phdr[i].p_memsz
      && !(((phdr[i].p_vaddr + phdr[i].p_memsz - 1)
	    ^ (phdr[i].p_vaddr + p_filesz - 1)) & ~(dso->arch->page_size - 1)))
    {
      for (j = 1; j < ehdr->e_shnum; ++j)
	{
	  if (!(shdr[j].sh_flags & (SHF_ALLOC | SHF_WRITE | SHF_ALLOC)))
	    break;
	  if (shdr[j].sh_type == SHT_NOBITS
	      && (shdr[j].sh_flags & SHF_TLS) == 0
	      && shdr[j].sh_addr >= phdr[i].p_vaddr)
	    shdr[j].sh_type = SHT_PROGBITS;
	}

      insert_readonly_section (ehdr, shdr, j, adjust);
      shdr[j] = *add;
      shdr[j].sh_addr = (shdr[j - 1].sh_addr + shdr[j - 1].sh_size
			 + add->sh_addralign - 1) & ~(add->sh_addralign - 1);
      shdr[j].sh_offset = (shdr[j].sh_addr - phdr[i].p_vaddr)
			  + phdr[i].p_offset;
      phdr[i].p_filesz = shdr[j].sh_addr + add->sh_size - phdr[i].p_vaddr;
      phdr[i].p_memsz = phdr[i].p_filesz;
      adjust_nonalloc (dso, ehdr, shdr, 0, 0, phdr[i].p_offset
		       + phdr[i].p_filesz - shdr[j + 1].sh_offset);
      return j;
    }

  /* See if we can decrease binary's base VMA and thus gain space.
     This trick is mainly useful for IA-32.  */
  for (i = 0; i < ehdr->e_phnum; ++i)
    if (phdr[i].p_type == PT_LOAD)
      break;

  addr = (add->sh_size + add->sh_addralign - 1 + phdr[i].p_align - 1)
	 & ~(phdr[i].p_align - 1);
  if (phdr[i].p_align <= dso->arch->page_size
      && phdr[i].p_flags == (PF_R | PF_X)
      && phdr[i].p_filesz == phdr[i].p_memsz
      && phdr[i].p_vaddr - addr
      && ! (((phdr[i].p_vaddr - addr) ^ phdr[i].p_vaddr)
	    & ~(phdr[i].p_align * 256 - 1)))
    {
      int moveend;
      if (! adjust->basemove_end)
	{
	  for (moveend = 1; moveend < ehdr->e_shnum; ++moveend)
	    if (strcmp (strptr (dso, ehdr->e_shstrndx,
				shdr[moveend].sh_name), ".interp")
		&& shdr[moveend].sh_type != SHT_NOTE)
	      break;
	  if (moveend < ehdr->e_shnum && moveend > 1)
	    {
	      adjust->basemove_end = shdr[moveend].sh_addr;
	      adjust->moveend = moveend;
	    }
	}
      else
	moveend = adjust->moveend;
      if (moveend < ehdr->e_shnum && moveend > 1
	  && (shdr[moveend].sh_flags & (SHF_ALLOC | SHF_WRITE)))
	{
	  int k = moveend;
	  GElf_Addr adj = addr;

	  if (add->sh_addr && ! adjust->move2
	      && phdr[i].p_vaddr <= add->sh_addr
	      && phdr[i].p_vaddr + phdr[i].p_filesz > add->sh_addr)
	    {
	      for (k = moveend; k < ehdr->e_shnum; ++k)
		{
		  if (! (shdr[k].sh_flags & (SHF_ALLOC | SHF_WRITE)))
		    {
		      k = ehdr->e_shnum;
		      break;
		    }

		  if (shdr[k].sh_addr > add->sh_addr)
		    {
		      /* Don't allow inserting in between reloc sections
			 if they are adjacent.  Similarly for adjacent
			 note sections.  */
		      int sh_type1 = (shdr[k - 1].sh_type == SHT_RELA)
				     ? SHT_REL : shdr[k - 1].sh_type;
		      int sh_type2 = (shdr[k].sh_type == SHT_RELA)
				     ? SHT_REL : shdr[k].sh_type;

		      if (sh_type1 != sh_type2)
			break;

		      if (sh_type1 != SHT_REL && sh_type1 != SHT_NOTE)
			break;
		      if ((shdr[k - 1].sh_addr
			   + ((shdr[k - 1].sh_size + 3) & -4))
			  != shdr[k].sh_addr)
			break;
		    }

		  if (! readonly_is_movable (dso, ehdr, shdr, k))
		    {
		      k = ehdr->e_shnum;
		      break;
		    }
		}

	      if (k < ehdr->e_shnum)
		{
		  GElf_Addr a;

		  a = shdr[k].sh_addr;
		  a -= shdr[k - 1].sh_addr + shdr[k - 1].sh_size;
		  assert (add->sh_addralign <= phdr[i].p_align);
		  assert (add->sh_size > a);
		  a = (add->sh_size - a + phdr[i].p_align - 1)
		      & ~(phdr[i].p_align - 1);
		  if (a < adj)
		    {
		      adjust->move2 = 1;
		      adj = a;
		    }
		  else
		    k = moveend;
		}
	      else
		k = moveend;
	    }

	  for (j = 1; j < k; ++j)
	    shdr[j].sh_addr -= adj;
	  phdr[i].p_vaddr -= adj;
	  phdr[i].p_paddr -= adj;
	  phdr[i].p_filesz += adj;
	  phdr[i].p_memsz += adj;
	  for (j = 0; j < ehdr->e_phnum; ++j)
	    {
	      if (j == i)
		continue;
	      /* Leave STACK segment alone, it has p_vaddr == p_paddr == 0
		 and p_offset == p_filesz == p_memsz == 0.  */
	      if (phdr[j].p_type == PT_GNU_STACK)
		continue;
	      if (phdr[j].p_vaddr
		  < adjust->basemove_end - adjust->basemove_adjust)
		{
		  phdr[j].p_vaddr -= adj;
		  phdr[j].p_paddr -= adj;
		}
	      else
		phdr[j].p_offset += adj;
	    }
	  adjust->basemove_adjust += adj;
	  insert_readonly_section (ehdr, shdr, k, adjust);
	  shdr[k] = *add;
	  if (k == moveend)
	    {
	      addr = shdr[k - 1].sh_addr + shdr[k - 1].sh_size;
	      addr = (addr + add->sh_addralign - 1) & ~(add->sh_addralign - 1);
	    }
	  else
	    {
	      addr = (shdr[k + 1].sh_addr - add->sh_size)
		     & ~(add->sh_addralign - 1);
	    }

	  shdr[k].sh_addr = addr;
	  shdr[k].sh_offset = (addr - phdr[i].p_vaddr) + phdr[i].p_offset;
	  adjust_nonalloc (dso, ehdr, shdr, 0, 0, adj);
	  return k;
	}
    }

  /* We have to create new PT_LOAD if at all possible.  */
  addr = ehdr->e_phoff + (ehdr->e_phnum + 1) * ehdr->e_phentsize;
  for (j = 1; j < ehdr->e_shnum; ++j)
    {
      if (addr > shdr[j].sh_offset)
	{
	  GElf_Addr start, addstart, endaddr, *old_addr;
	  GElf_Addr minsize = ~(GElf_Addr) 0;
	  int movesec = -1, last, k, e;

	  if (ehdr->e_phoff < phdr[i].p_offset
	      || ehdr->e_phoff + (ehdr->e_phnum + 1) * ehdr->e_phentsize
		 > phdr[i].p_offset + phdr[i].p_filesz
	      || ! readonly_is_movable (dso, ehdr, shdr, j)
	      || shdr[j].sh_addr >= phdr[i].p_vaddr + phdr[i].p_filesz)
	    {
	      error (0, 0, "%s: No space in ELF segment table to add new ELF segment",
		     dso->filename);
	      return 0;
	    }

	  start = phdr[i].p_vaddr - phdr[i].p_offset + ehdr->e_phoff
		  + (ehdr->e_phnum + 1) * ehdr->e_phentsize;
	  for (last = 1; last < ehdr->e_shnum; ++last)
	    if (! readonly_is_movable (dso, ehdr, shdr, last)
		|| shdr[last].sh_addr >= phdr[i].p_vaddr + phdr[i].p_filesz)
	      break;
	  for (j = 1; j < last; ++j)
	    {
	      addstart = (start + add->sh_addralign - 1)
			 & ~(add->sh_addralign - 1);
	      start = (start + shdr[j].sh_addralign - 1)
		      & ~(shdr[j].sh_addralign - 1);
	      endaddr = -1;
	      if (j + 1 < ehdr->e_shnum)
		endaddr = shdr[j + 1].sh_addr;
	      if (phdr[i].p_vaddr + phdr[i].p_filesz < endaddr)
		endaddr = phdr[i].p_vaddr + phdr[i].p_filesz;

	      switch (shdr[j].sh_type)
		{
		case SHT_HASH:
		case SHT_GNU_HASH:
		case SHT_DYNSYM:
		case SHT_STRTAB:
		case SHT_GNU_verdef:
		case SHT_GNU_verneed:
		case SHT_GNU_versym:
		case SHT_GNU_LIBLIST:
		  if (endaddr >= start
		      && endaddr - start < minsize)
		    {
		      minsize = endaddr - start;
		      movesec = j;
		    }
		  if (endaddr > addstart
		      && endaddr - addstart > add->sh_size
		      && endaddr - addstart - add->sh_size
			 < minsize)
		    {
		      minsize = endaddr - addstart - add->sh_size;
		      movesec = j;
		    }
		  break;
		case SHT_REL:
		case SHT_RELA:
		case SHT_NOTE:
		    /* Don't allow inserting in between reloc sections
		       if they are adjacent.  Similarly for adjacent
		       note sections.  */
		    if (j + 1 < ehdr->e_shnum)
		      {
			if (shdr[j].sh_type == SHT_NOTE)
			  {
			    if (shdr[j + 1].sh_type != SHT_NOTE)
			      break;
			  }
			else if (shdr[j + 1].sh_type != SHT_REL
				 && shdr[j + 1].sh_type != SHT_RELA)
			  {
			    break;
			  }

			if ((shdr[j].sh_addr
			     + ((shdr[j].sh_size + 3) & -4))
			    != shdr[j + 1].sh_addr)
			  break;

			start += shdr[j].sh_size;
			continue;
		      }
		    break;
		}

	      if (start + shdr[j].sh_size <= endaddr)
		{
		  movesec = j + 1;
		  break;
		}
	      start += shdr[j].sh_size;
	    }

	  if (movesec == -1)
	    {
	      error (0, 0, "%s: No space in ELF segment table to add new ELF segment",
		     dso->filename);
	      return 0;
	    }

	  start = phdr[i].p_vaddr - phdr[i].p_offset + ehdr->e_phoff
		  + (ehdr->e_phnum + 1) * ehdr->e_phentsize;
	  old_addr = (GElf_Addr *) alloca (movesec * sizeof (GElf_Addr));
	  for (k = 1; k < movesec; ++k)
	    {
	      start = (start + shdr[k].sh_addralign - 1)
		      & ~(shdr[k].sh_addralign - 1);
	      old_addr[k] = shdr[k].sh_addr;
	      shdr[k].sh_addr = start;
	      shdr[k].sh_offset = start + phdr[i].p_offset
				  - phdr[i].p_vaddr;
	      start += shdr[k].sh_size;
	    }

	  for (e = 0; e < ehdr->e_phnum; ++e)
	    if (phdr[e].p_type != PT_LOAD
		&& phdr[e].p_type != PT_GNU_STACK)
	      for (k = 1; k < movesec; ++k)
		if (old_addr[k] == phdr[e].p_vaddr)
		  {
		    if (phdr[e].p_filesz != shdr[k].sh_size
			|| phdr[e].p_memsz != shdr[k].sh_size)
		      {
			int k1 = -1;
			if (phdr[e].p_type == PT_NOTE
			    && shdr[k].sh_type == SHT_NOTE
			    && phdr[e].p_filesz == phdr[e].p_memsz)
			  {
			    k1 = k;
			    while (k1 < movesec)
			      {
				if (shdr[k1].sh_type != SHT_NOTE
				    || shdr[k1].sh_addr - old_addr[k1]
				       != shdr[k].sh_addr - old_addr[k]
				    || old_addr[k1] + shdr[k1].sh_size
				       > phdr[e].p_vaddr + phdr[e].p_filesz)
				  {
				    k1 = -1;
				    break;
				  }
				if (old_addr[k1] + shdr[k1].sh_size
				    == phdr[e].p_vaddr + phdr[e].p_filesz)
				  break;
				++k1;
			      }
			    if (k1 == movesec)
			      k1 = -1;
			  }
			if (k1 == -1)
			  {
			    error (0, 0, "%s: Non-PT_LOAD segment spanning more than one section",
				   dso->filename);
			    return 0;
			  }
		      }
		    phdr[e].p_vaddr += shdr[k].sh_addr - old_addr[k];
		    phdr[e].p_paddr += shdr[k].sh_addr - old_addr[k];
		    phdr[e].p_offset += shdr[k].sh_addr - old_addr[k];
		    break;
		  }

	  if (j < last)
	    /* Now continue as if there was place for a new PT_LOAD
	       in ElfW(Phdr) table initially.  */
	    break;
	  else
	    {
	      GElf_Shdr moveshdr;
	      int newidx, ret, movedidx, oldidx;

	      moveshdr = shdr[movesec];
	      newidx = remove_readonly_section (ehdr, shdr, movesec, adjust);
	      oldidx = adjust->move->new_to_old[movesec];
	      remove_section (adjust->move, movesec);
	      ret = find_readonly_space (dso, add, ehdr, phdr, shdr, adjust);
	      if (ret == 0)
		return 0;
	      movedidx = find_readonly_space (dso, &moveshdr, ehdr, phdr,
					      shdr, adjust);
	      if (movedidx == 0)
		return 0;
	      if (newidx != -1)
		adjust->new[newidx] = movedidx;
	      add_section (adjust->move, movedidx);
	      if (oldidx != -1)
		{
		  adjust->move->old_to_new[oldidx] = movedidx;
		  adjust->move->new_to_old[movedidx] = oldidx;
		}
	      if (movedidx <= ret)
		++ret;
	      return ret;
	    }
	}
    }

  for (i = 0, j = 0; i < ehdr->e_phnum; ++i)
    if (phdr[i].p_type == PT_LOAD)
      j = i;
    else if (phdr[i].p_type == PT_PHDR)
      {
	if (phdr[i].p_filesz == ehdr->e_phnum * ehdr->e_phentsize)
	  phdr[i].p_filesz += ehdr->e_phentsize;
	if (phdr[i].p_memsz == ehdr->e_phnum * ehdr->e_phentsize)
	  phdr[i].p_memsz += ehdr->e_phentsize;
      }

  memmove (&phdr[j + 2], &phdr[j + 1],
	   (ehdr->e_phnum - j - 1) * sizeof (GElf_Phdr));
  ++ehdr->e_phnum;
  phdr[++j].p_type = PT_LOAD;
  phdr[j].p_offset = phdr[j - 1].p_offset + phdr[j - 1].p_filesz;
  phdr[j].p_offset = (phdr[j].p_offset + add->sh_addralign - 1)
		      & ~(add->sh_addralign - 1);
  phdr[j].p_align = phdr[j - 1].p_align;
  phdr[j].p_vaddr = phdr[j - 1].p_vaddr + phdr[j - 1].p_memsz;
  phdr[j].p_vaddr += (phdr[j].p_align - 1);
  phdr[j].p_vaddr &= ~(phdr[j].p_align - 1);
  phdr[j].p_vaddr += (phdr[j].p_offset & (phdr[j].p_align - 1));
  phdr[j].p_paddr = phdr[j].p_vaddr;
  /* Although the content of the segment is read-only, unless it ends on
     a page boundary, we must make it writeable. This is because the rest of
     the last page in the segment will be used as sbrk area which is assumed
     to be writeable.  */
  phdr[j].p_flags = (PF_R | PF_W);
  phdr[j].p_filesz = add->sh_size;
  phdr[j].p_memsz = add->sh_size;
  for (i = 1; i < ehdr->e_shnum; ++i)
    if (! (shdr[i].sh_flags & (SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR)))
      break;
  assert (i < ehdr->e_shnum);
  insert_readonly_section (ehdr, shdr, i, adjust);
  shdr[i] = *add;
  shdr[i].sh_addr = phdr[j].p_vaddr;
  shdr[i].sh_offset = phdr[j].p_offset;
  adjust_nonalloc (dso, ehdr, shdr, 0, 0,
		   phdr[j].p_offset + phdr[j].p_filesz - phdr[j - 1].p_offset
		   - phdr[j - 1].p_filesz);
  return i;
}
