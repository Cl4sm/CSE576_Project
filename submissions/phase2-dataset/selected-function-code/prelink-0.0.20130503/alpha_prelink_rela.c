alpha_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		    GElf_Addr relaaddr)
{
  DSO *dso;
  GElf_Addr value;

  if (GELF_R_TYPE (rela->r_info) == R_ALPHA_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_ALPHA_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  dso = info->dso;
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_ALPHA_GLOB_DAT:
    case R_ALPHA_REFQUAD:
    case R_ALPHA_DTPREL64:
      write_le64 (dso, rela->r_offset, value);
      break;
    case R_ALPHA_JMP_SLOT:
      write_le64 (dso, rela->r_offset, value);
      alpha_fixup_plt (dso, rela, relaaddr, value);
      break;
    /* DTPMOD64 and TPREL64 is impossible to predict in shared libraries
       unless prelink sets the rules.  */
    case R_ALPHA_DTPMOD64:
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  error (0, 0, "%s: R_ALPHA_DTPMOD64 reloc in executable?",
		 dso->filename);
	  return 1;
	}
      break;
    case R_ALPHA_TPREL64:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	write_le64 (dso, rela->r_offset, value + info->resolvetls->offset);
      break;
    default:
      error (0, 0, "%s: Unknown alpha relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
