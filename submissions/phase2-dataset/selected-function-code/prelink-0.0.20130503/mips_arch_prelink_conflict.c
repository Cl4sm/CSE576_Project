static int
mips_arch_prelink_conflict (DSO *dso, struct prelink_info *info)
{
  struct mips_global_got_iterator ggi;
  GElf_Addr value;
  struct prelink_conflict *conflict;
  GElf_Rela *entry;

  if (info->dso == dso || dso->info[DT_PLTGOT] == 0)
    return 0;

  /* Add a conflict for every global GOT entry that does not hold the
     right value, either because of a conflict, or because the DSO has
     a lazy binding stub for a symbol that it also defines.  */
  mips_init_global_got_iterator (&ggi, dso);
  while (mips_get_global_got_entry (&ggi))
    {
      conflict = prelink_conflict (info, ggi.sym_index, R_MIPS_REL32);
      if (conflict != NULL)
	value = conflict_lookup_value (conflict);
      else if (ggi.sym.st_shndx != SHN_UNDEF
	       && ggi.sym.st_shndx != SHN_COMMON)
	value = ggi.sym.st_value;
      else
	continue;
      if (buf_read_une32 (dso, ggi.got_entry) != value)
	{
	  entry = prelink_conflict_add_rela (info);
	  if (entry == NULL)
	    return 1;
	  entry->r_addend = (int32_t) value;
	  entry->r_offset = ggi.got_addr;
	  entry->r_info = GELF_R_INFO (0, R_MIPS_REL32);
	}
    }

  return ggi.failed;
}
