int
update_dynamic_tags (DSO *dso, GElf_Shdr *shdr, GElf_Shdr *old_shdr,
		     struct section_move *move)
{
  int i, j;

  for (i = 1; i < move->new_shnum; ++i)
    {
      j = move->new_to_old[i];
      if (j == -1)
	continue;
      if ((dynamic_info_is_set (dso, DT_HASH)
	   && dso->info[DT_HASH] == old_shdr[j].sh_addr
	   && old_shdr[j].sh_type == SHT_HASH
	   && set_dynamic (dso, DT_HASH, shdr[i].sh_addr, 1))
	  || (dynamic_info_is_set (dso, DT_SYMTAB)
	      && dso->info[DT_SYMTAB] == old_shdr[j].sh_addr
	      && old_shdr[j].sh_type == SHT_DYNSYM
	      && set_dynamic (dso, DT_SYMTAB, shdr[i].sh_addr, 1))
	  || (dynamic_info_is_set (dso, DT_STRTAB)
	      && dso->info[DT_STRTAB] == old_shdr[j].sh_addr
	      && old_shdr[j].sh_type == SHT_STRTAB
	      && set_dynamic (dso, DT_STRTAB, shdr[i].sh_addr, 1))
	  || (dynamic_info_is_set (dso, DT_VERDEF_BIT)
	      && dso->info_DT_VERDEF == old_shdr[j].sh_addr
	      && old_shdr[j].sh_type == SHT_GNU_verdef
	      && set_dynamic (dso, DT_VERDEF, shdr[i].sh_addr, 1))
	  || (dynamic_info_is_set (dso, DT_VERNEED_BIT)
	      && dso->info_DT_VERNEED == old_shdr[j].sh_addr
	      && old_shdr[j].sh_type == SHT_GNU_verneed
	      && set_dynamic (dso, DT_VERNEED, shdr[i].sh_addr, 1))
	  || (dynamic_info_is_set (dso, DT_VERSYM_BIT)
	      && dso->info_DT_VERSYM == old_shdr[j].sh_addr
	      && old_shdr[j].sh_type == SHT_GNU_versym
	      && set_dynamic (dso, DT_VERSYM, shdr[i].sh_addr, 1))
	  || (dynamic_info_is_set (dso, DT_GNU_HASH_BIT)
	      && dso->info_DT_GNU_HASH == old_shdr[j].sh_addr
	      && old_shdr[j].sh_type == SHT_GNU_HASH
	      && set_dynamic (dso, DT_GNU_HASH, shdr[i].sh_addr, 1)))
	return 1;
    }

  return 0;
}
