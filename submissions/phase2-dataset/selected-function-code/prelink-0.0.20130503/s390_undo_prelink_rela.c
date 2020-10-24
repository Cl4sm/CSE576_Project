static int
s390_undo_prelink_rela (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr)
{
  int sec;
  const char *name;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_390_NONE:
    case R_390_RELATIVE:
    case R_390_IRELATIVE:
      break;
    case R_390_JMP_SLOT:
      sec = addr_to_sec (dso, rela->r_offset);
      name = strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[sec].sh_name);
      if (sec == -1 || (strcmp (name, ".got") && strcmp (name, ".got.plt")))
	{
	  error (0, 0, "%s: R_390_JMP_SLOT not pointing into .got section",
		 dso->filename);
	  return 1;
	}
      else
	{
	  Elf32_Addr data = read_ube32 (dso, dso->shdr[sec].sh_addr + 4);

	  assert (rela->r_offset >= dso->shdr[sec].sh_addr + 12);
	  assert (((rela->r_offset - dso->shdr[sec].sh_addr) & 3) == 0);
	  write_be32 (dso, rela->r_offset,
		      8 * (rela->r_offset - dso->shdr[sec].sh_addr - 12)
		      + data);
	}
      break;
    case R_390_GLOB_DAT:
    case R_390_32:
    case R_390_PC32:
    case R_390_PC32DBL:
    case R_390_PLT32DBL:
    case R_390_TLS_DTPMOD:
    case R_390_TLS_DTPOFF:
    case R_390_TLS_TPOFF:
      write_be32 (dso, rela->r_offset, 0);
      break;
    case R_390_16:
    case R_390_PC16:
    case R_390_PC16DBL:
    case R_390_PLT16DBL:
      write_be16 (dso, rela->r_offset, 0);
      break;
    case R_390_8:
      write_8 (dso, rela->r_offset, 0);
      break;
    case R_390_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_390_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown s390 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
