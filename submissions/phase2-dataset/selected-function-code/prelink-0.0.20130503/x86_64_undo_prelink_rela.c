x86_64_undo_prelink_rela (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr)
{
  int sec;
  const char *name;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_X86_64_NONE:
    case R_X86_64_RELATIVE:
    case R_X86_64_IRELATIVE:
      break;
    case R_X86_64_JUMP_SLOT:
      sec = addr_to_sec (dso, rela->r_offset);
      name = strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[sec].sh_name);
      if (sec == -1 || (strcmp (name, ".got") && strcmp (name, ".got.plt")))
	{
	  error (0, 0, "%s: R_X86_64_JUMP_SLOT not pointing into .got section",
		 dso->filename);
	  return 1;
	}
      else
	{
	  Elf64_Addr data = read_ule64 (dso, dso->shdr[sec].sh_addr + 8);

	  assert (rela->r_offset >= dso->shdr[sec].sh_addr + 24);
	  assert (((rela->r_offset - dso->shdr[sec].sh_addr) & 7) == 0);
	  write_le64 (dso, rela->r_offset,
		      2 * (rela->r_offset - dso->shdr[sec].sh_addr - 24)
		      + data);
	}
      break;
    case R_X86_64_GLOB_DAT:
    case R_X86_64_64:
    case R_X86_64_DTPMOD64:
    case R_X86_64_DTPOFF64:
    case R_X86_64_TPOFF64:
      write_le64 (dso, rela->r_offset, 0);
      break;
    case R_X86_64_32:
    case R_X86_64_PC32:
      write_le32 (dso, rela->r_offset, 0);
      break;
    case R_X86_64_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_X86_64_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown X86-64 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
