adjust_stabs (DSO *dso, int n, GElf_Addr start, GElf_Addr adjust)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  off_t off;
  uint32_t (*read_32) (char *p);
  void (*write_32) (char *p, uint32_t v);
  uint32_t value;
  int sec, type;

  assert (dso->shdr[n].sh_entsize == 12);
  data = elf_getdata (scn, NULL);
  assert (data != NULL && data->d_buf != NULL);
  assert (elf_getdata (scn, data) == NULL);
  assert (data->d_off == 0 && data->d_size == dso->shdr[n].sh_size);
#if __BYTE_ORDER == __BIG_ENDIAN
  if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
#elif __BYTE_ORDER == __LITTLE_ENDIAN
  if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2LSB)
#else
# error Not supported host endianess
#endif
    {
      read_32 = read_native;
      write_32 = write_native;
    }
#if __BYTE_ORDER == __BIG_ENDIAN
  else if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2LSB)
#elif __BYTE_ORDER == __LITTLE_ENDIAN
  else if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
#endif
    {
      read_32 = read_swap;
      write_32 = write_swap;
    }
  else
    {
      error (0, 0, "%s: Wrong ELF data enconding", dso->filename);
      return 1;
    }

  for (off = 0; off < data->d_size; off += 12)
    {
    switch ((type = *(uint8_t *)(data->d_buf + off + 4)))
      {
      case N_FUN:
	/* If string is "", N_FUN is function length, otherwise
	   it is function start address.  */
	if (read_32 (data->d_buf + off) == 0)
	  break;
	/* FALLTHROUGH */
      case N_STSYM:
      case N_LCSYM:
      case N_CATCH:
      case N_SO:
      case N_SOL:
      case N_BNSYM:
      case N_ENSYM:
	value = read_32 (data->d_buf + off + 8);
	sec = addr_to_sec (dso, value);
	if (sec != -1)
	  {
	    addr_adjust (value, start, adjust);
	    write_32 (data->d_buf + off + 8, value);
	  }
	break;
      /* These should be always 0.  */
      case N_GSYM:
      case N_BINCL:
      case N_EINCL:
      case N_EXCL:
      case N_BCOMM:
      case N_ECOMM:
      /* These contain other values.  */
      case N_ZERO:
      case N_NSYMS:
      case N_NOMAP:
      case N_RSYM:
      case N_LSYM:
      case N_PSYM:
      case N_OPT:
      /* These are relative.  */
      case N_LBRAC:
      case N_RBRAC:
      case N_SLINE:
      case N_BSLINE:
      case N_DSLINE:
	break;
      default:
	error (0, 0, "%s: Unknown stabs code 0x%02x\n", dso->filename, type);
	return 1;
      }
    }

  elf_flagscn (scn, ELF_C_SET, ELF_F_DIRTY);
  return 0;
}
