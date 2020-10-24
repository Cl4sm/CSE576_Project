mips_prelink_rel (struct prelink_info *info, GElf_Rel *rel, GElf_Addr reladdr)
{
  DSO *dso;

  /* Convert R_MIPS_REL32 relocations against global symbols into
     R_MIPS_GLOB_DAT if the addend is zero.  */
  dso = info->dso;
  if (GELF_R_TYPE (rel->r_info) == R_MIPS_REL32
      && GELF_R_SYM (rel->r_info) >= dso->info_DT_MIPS_GOTSYM
      && read_une32 (dso, rel->r_offset) == 0)
    {
      rel->r_info = GELF_R_INFO (GELF_R_SYM (rel->r_info), R_MIPS_GLOB_DAT);
      write_ne32 (dso, rel->r_offset,
		  info->resolve (info, GELF_R_SYM (rel->r_info),
				 GELF_R_TYPE (rel->r_info)));
      return 2;
    }
  return mips_prelink_reloc (info, rel->r_offset, rel->r_info, NULL);
}
