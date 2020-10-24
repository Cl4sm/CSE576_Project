static GElf_Addr
resolve_ldso (struct prelink_info *info, GElf_Word r_sym,
	      int reloc_type __attribute__((unused)))
{
  /* Dynamic linker does not depend on any other library,
     all symbols resolve to themselves with the exception
     of SHN_UNDEF symbols which resolve to 0.  */
  if (info->symtab[r_sym].st_shndx == SHN_UNDEF)
    {
      info->resolveent = NULL;
      info->resolvetls = NULL;
      return 0;
    }
  else
    {
      /* As the dynamic linker is relocated first,
	 l_addr will be 0.  */
      info->resolveent = info->ent;
      info->resolvetls = NULL;
      return 0 + info->symtab[r_sym].st_value;
    }
}
