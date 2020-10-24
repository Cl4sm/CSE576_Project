prelink_add_copy_rel (DSO *dso, int n, GElf_Rel *rel, struct copy_relocs *cr)
{
  Elf_Data *data = NULL;
  int symsec = dso->shdr[n].sh_link;
  Elf_Scn *scn = dso->scn[symsec];
  GElf_Sym sym;
  size_t entsize = dso->shdr[symsec].sh_entsize;
  off_t off = GELF_R_SYM (rel->r_info) * entsize;

  while ((data = elf_getdata (scn, data)) != NULL)
    {
      if (data->d_off <= off &&
	  data->d_off + data->d_size >= off + entsize)
	{
	  gelfx_getsym (dso->elf, data, (off - data->d_off) / entsize, &sym);
	  if (sym.st_size == 0)
	    {
	      error (0, 0, "%s: Copy reloc against symbol with zero size",
		     dso->filename);
	      return 1;
	    }

	  if (cr->alloced == cr->count)
	    {
	      cr->alloced += 10;
	      cr->rela = realloc (cr->rela, cr->alloced * sizeof (GElf_Rela));
	      if (cr->rela == NULL)
		{
		  error (0, ENOMEM, "%s: Could not build list of COPY relocs",
			 dso->filename);
		  return 1;
		}
	    }
	  cr->rela[cr->count].r_offset = rel->r_offset;
	  cr->rela[cr->count].r_info = rel->r_info;
	  cr->rela[cr->count].r_addend = sym.st_size;
	  ++cr->count;
	  return 0;
	}
    }

  error (0, 0, "%s: Copy reloc against unknown symbol", dso->filename);
  return 1;
}
