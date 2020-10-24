arm_undo_prelink_rel (DSO *dso, GElf_Rel *rel, GElf_Addr reladdr)
{
  int sec;
  const char *name;

  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_ARM_RELATIVE:
    case R_ARM_NONE:
      break;
    case R_ARM_JUMP_SLOT:
      sec = addr_to_sec (dso, rel->r_offset);
      name = strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[sec].sh_name);
      if (sec == -1 || (strcmp (name, ".got") && strcmp (name, ".got.plt")))
	{
	  error (0, 0, "%s: R_ARM_JMP_SLOT not pointing into .got section",
		 dso->filename);
	  return 1;
	}
      else
	{
	  Elf32_Addr data = read_une32 (dso, dso->shdr[sec].sh_addr + 4);

	  assert (rel->r_offset >= dso->shdr[sec].sh_addr + 12);
	  assert (((rel->r_offset - dso->shdr[sec].sh_addr) & 3) == 0);
	  write_ne32 (dso, rel->r_offset, data);
	}
      break;
    case R_ARM_GLOB_DAT:
      sec = addr_to_sec (dso, rel->r_offset);

      write_ne32 (dso, rel->r_offset, 0);
      if (sec != -1)
	{
	  if (strcmp (strptr (dso, dso->ehdr.e_shstrndx,
			      dso->shdr[sec].sh_name),
		      ".got"))
	    {
	      rel->r_info = GELF_R_INFO (GELF_R_SYM (rel->r_info), R_ARM_ABS32);
	      return 2;
	    }
	}
      break;
    case R_ARM_ABS32:
    case R_ARM_PC24:
      error (0, 0, "%s: R_ARM_%s relocs should not be present in prelinked REL sections",
	     GELF_R_TYPE (rel->r_info) == R_ARM_ABS32 ? "ABS32" : "PC24",
	     dso->filename);
      return 1;
    case R_ARM_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_ARM_COPY reloc in shared library?", dso->filename);
      return 1;
    case R_ARM_TLS_DTPMOD32:
    case R_ARM_TLS_DTPOFF32:
      write_ne32 (dso, rel->r_offset, 0);
      break;
    case R_ARM_TLS_TPOFF32:
      break;
    default:
      error (0, 0, "%s: Unknown arm relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rel->r_info));
      return 1;
    }
  return 0;
}
