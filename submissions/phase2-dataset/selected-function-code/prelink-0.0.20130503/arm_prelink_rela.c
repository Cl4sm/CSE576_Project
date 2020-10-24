static int
arm_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		  GElf_Addr relaaddr)
{
  DSO *dso;
  GElf_Addr value;
  Elf32_Sword val;

  if (GELF_R_TYPE (rela->r_info) == R_ARM_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_ARM_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  dso = info->dso;
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_ARM_GLOB_DAT:
    case R_ARM_JUMP_SLOT:
      write_ne32 (dso, rela->r_offset, value + rela->r_addend);
      break;
    case R_ARM_ABS32:
      write_ne32 (dso, rela->r_offset, value + rela->r_addend);
      break;
    case R_ARM_PC24:
      val = value + rela->r_addend - rela->r_offset;
      val >>= 2;
      if ((Elf32_Word) val + 0x800000 >= 0x1000000)
	{
	  error (0, 0, "%s: R_ARM_PC24 overflow", dso->filename);
	  return 1;
	}
      val &= 0xffffff;
      write_ne32 (dso, rela->r_offset,
		  (read_une32 (dso, rela->r_offset) & 0xff000000) | val);
      break;
    case R_ARM_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_ARM_COPY reloc in shared library?", dso->filename);
      return 1;
    case R_ARM_TLS_DTPOFF32:
      write_ne32 (dso, rela->r_offset, value + rela->r_addend);
      break;
    /* DTPMOD32 and TPOFF32 is impossible to predict in shared libraries
       unless prelink sets the rules.  */
    case R_ARM_TLS_DTPMOD32:
      if (dso->ehdr.e_type == ET_EXEC)
        {
          error (0, 0, "%s: R_ARM_TLS_DTPMOD32 reloc in executable?",
                 dso->filename);
          return 1;
        }
      break;
    case R_ARM_TLS_TPOFF32:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
        write_ne32 (dso, rela->r_offset,
                    value + rela->r_addend + info->resolvetls->offset);
      break;
    default:
      error (0, 0, "%s: Unknown arm relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
