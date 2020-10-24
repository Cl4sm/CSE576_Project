mips_prelink_reloc (struct prelink_info *info, GElf_Addr r_offset,
		    GElf_Xword r_info, GElf_Rela *rela)
{
  DSO *dso;
  GElf_Addr value;
  GElf_Word r_sym;
  int r_type;

  dso = info->dso;
  r_sym = GELF_R_SYM (r_info);
  r_type = GELF_R_TYPE (r_info);
  switch (r_type)
    {
    case R_MIPS_NONE:
      break;

    case R_MIPS_REL32:
      /* An in-place R_MIPS_REL32 relocation against symbol 0 needs no
	 adjustment.  */
      if (rela != NULL || GELF_R_SYM (r_info) != 0)
	{
	  value = info->resolve (info, r_sym, r_type);
	  mips_prelink_32bit_reloc (dso, rela, value);
	}
      break;

    case R_MIPS_GLOB_DAT:
      write_ne32 (dso, r_offset, info->resolve (info, r_sym, r_type));
      break;

    case R_MIPS_TLS_DTPMOD32:
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  error (0, 0, "%s: R_MIPS_TLS_DTPMOD32 reloc in executable?",
		 dso->filename);
	  return 1;
	}
      /* These relocations will be resolved using a conflict.  We need
	 not change the field value here.  */
      break;

    case R_MIPS_TLS_DTPREL32:
      value = info->resolve (info, r_sym, r_type);
      mips_prelink_32bit_reloc (dso, rela, value - TLS_DTV_OFFSET);
      break;

    case R_MIPS_TLS_TPREL32:
      /* Relocations in a shared library will be resolved using a conflict.
	 We need not change the relocation field here.  */
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  value = info->resolve (info, r_sym, r_type);
	  value += info->resolvetls->offset - TLS_TP_OFFSET;
	  mips_prelink_32bit_reloc (dso, rela, value);
	}
      break;

    default:
      error (0, 0, "%s: Unknown MIPS relocation type %d",
	     dso->filename, (int) GELF_R_TYPE (r_info));
      return 1;
    }
  return 0;
}
