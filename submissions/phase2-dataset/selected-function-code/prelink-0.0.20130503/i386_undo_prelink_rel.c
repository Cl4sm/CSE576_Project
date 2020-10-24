static int
i386_undo_prelink_rel (DSO *dso, GElf_Rel *rel, GElf_Addr reladdr)
{
  int sec;
  const char *name;

  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_386_NONE:
    case R_386_RELATIVE:
    case R_386_IRELATIVE:
      break;
    case R_386_JMP_SLOT:
      sec = addr_to_sec (dso, rel->r_offset);
      name = strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[sec].sh_name);
      if (sec == -1 || (strcmp (name, ".got") && strcmp (name, ".got.plt")))
	{
	  error (0, 0, "%s: R_386_JMP_SLOT not pointing into .got section",
		 dso->filename);
	  return 1;
	}
      else
	{
	  Elf32_Addr data = read_ule32 (dso, dso->shdr[sec].sh_addr + 4);

	  assert (rel->r_offset >= dso->shdr[sec].sh_addr + 12);
	  assert (((rel->r_offset - dso->shdr[sec].sh_addr) & 3) == 0);
	  write_le32 (dso, rel->r_offset,
		      4 * (rel->r_offset - dso->shdr[sec].sh_addr - 12)
		      + data);
	}
      break;
    case R_386_GLOB_DAT:
      sec = addr_to_sec (dso, rel->r_offset);

      write_le32 (dso, rel->r_offset, 0);
      if (sec != -1)
	{
	  if (strcmp (strptr (dso, dso->ehdr.e_shstrndx,
			      dso->shdr[sec].sh_name),
		      ".got"))
	    {
	      rel->r_info = GELF_R_INFO (GELF_R_SYM (rel->r_info), R_386_32);
	      return 2;
	    }
	}
      break;
    case R_386_32:
    case R_386_PC32:
      error (0, 0, "%s: R_386_%s32 relocs should not be present in prelinked REL sections",
	     GELF_R_TYPE (rel->r_info) == R_386_32 ? "" : "PC", dso->filename);
      return 1;
    case R_386_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_386_COPY reloc in shared library?", dso->filename);
      return 1;
    case R_386_TLS_DTPMOD32:
    case R_386_TLS_DTPOFF32:
      write_le32 (dso, rel->r_offset, 0);
      break;
    case R_386_TLS_TPOFF32:
    case R_386_TLS_TPOFF:
      break;
    default:
      error (0, 0, "%s: Unknown i386 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rel->r_info));
      return 1;
    }
  return 0;
}
