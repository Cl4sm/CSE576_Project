static int
sh_undo_prelink_rela (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr)
{
  int sec;
  const char *name;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_SH_NONE:
      break;
    case R_SH_RELATIVE:
      if (rela->r_addend)
	write_le32 (dso, rela->r_offset, 0);
      break;
    case R_SH_JMP_SLOT:
      sec = addr_to_sec (dso, rela->r_offset);
      name = strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[sec].sh_name);
      if (sec == -1 || (strcmp (name, ".got") && strcmp (name, ".got.plt")))
	{
	  error (0, 0, "%s: R_SH_JMP_SLOT not pointing into .got section",
		 dso->filename);
	  return 1;
	}
      else
	{
	  Elf32_Addr data = read_une32 (dso, dso->shdr[sec].sh_addr + 4);

	  assert (rela->r_offset >= dso->shdr[sec].sh_addr + 12);
	  assert (((rela->r_offset - dso->shdr[sec].sh_addr) & 3) == 0);
	  write_ne32 (dso, rela->r_offset,
		      7 * (rela->r_offset - dso->shdr[sec].sh_addr - 12)
		      + data);
	}
      break;
    case R_SH_GLOB_DAT:
    case R_SH_DIR32:
    case R_SH_REL32:
      write_ne32 (dso, rela->r_offset, 0);
      break;
    case R_SH_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_SH_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown sh relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
