static int
cris_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		   GElf_Addr relaaddr)
{
  DSO *dso;
  GElf_Addr value;

  dso = info->dso;
  if (GELF_R_TYPE (rela->r_info) == R_CRIS_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  else if (GELF_R_TYPE (rela->r_info) == R_CRIS_RELATIVE)
    {
      write_le32 (dso, rela->r_offset, rela->r_addend);
      return 0;
    }
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_CRIS_GLOB_DAT:
    case R_CRIS_JUMP_SLOT:
    case R_CRIS_32:
      write_le32 (dso, rela->r_offset, value);
      break;
    case R_CRIS_16:
      write_le16 (dso, rela->r_offset, value);
      break;
    case R_CRIS_8:
      write_8 (dso, rela->r_offset, value);
      break;
    case R_CRIS_32_PCREL:
      write_le32 (dso, rela->r_offset, value - rela->r_offset - 4);
      break;
    case R_CRIS_16_PCREL:
      write_le16 (dso, rela->r_offset, value - rela->r_offset - 2);
      break;
    case R_CRIS_8_PCREL:
      write_8 (dso, rela->r_offset, value - rela->r_offset - 1);
      break;
    case R_CRIS_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_CRIS_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown cris relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
