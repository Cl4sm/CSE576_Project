start_mdebug (DSO *dso, int n, struct mdebug *mdebug)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];

  data = elf_getdata (scn, NULL);
  mdebug->buf = data->d_buf;
  mdebug->dso = dso;
  assert (data != NULL && data->d_buf != NULL);
  assert (elf_getdata (scn, data) == NULL);
  assert (data->d_off == 0 && data->d_size == dso->shdr[n].sh_size);
  if (dso->mdebug_orig_offset == 0)
    dso->mdebug_orig_offset = dso->shdr[n].sh_offset;
#if __BYTE_ORDER == __BIG_ENDIAN
  if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
#elif __BYTE_ORDER == __LITTLE_ENDIAN
  if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2LSB)
#else
# error Not supported host endianess
#endif
    {
      mdebug->read_32 = read_native_32;
      if (dso->arch->class == ELFCLASS32)
	{
	  mdebug->read_ptr = read_native_ptr32;
	  mdebug->write_ptr = write_native_ptr32;
	}
      else
	{
	  mdebug->read_ptr = read_native_ptr64;
	  mdebug->write_ptr = write_native_ptr64;
	}
    }
#if __BYTE_ORDER == __BIG_ENDIAN
  else if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2LSB)
#elif __BYTE_ORDER == __LITTLE_ENDIAN
  else if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
#endif
    {
      mdebug->read_32 = read_swap_32;
      if (dso->arch->class == ELFCLASS32)
	{
	  mdebug->read_ptr = read_swap_ptr32;
	  mdebug->write_ptr = write_swap_ptr32;
	}
      else
	{
	  mdebug->read_ptr = read_swap_ptr64;
	  mdebug->write_ptr = write_swap_ptr64;
	}
    }
  else
    {
      error (0, 0, "%s: Wrong ELF data enconding", dso->filename);
      return 1;
    }
  if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2LSB)
    {
      if (dso->arch->class == ELFCLASS32)
	mdebug->adjust_sym = (void *) adjust_mdebug_sym_le32;
      else
	mdebug->adjust_sym = (void *) adjust_mdebug_sym_le64;
    }
  else
    {
      if (dso->arch->class == ELFCLASS32)
	mdebug->adjust_sym = (void *) adjust_mdebug_sym_be32;
      else
	mdebug->adjust_sym = (void *) adjust_mdebug_sym_be64;
    }

  if (dso->shdr[n].sh_size < SIZEOF (mdebug_hdr))
    {
      error (0, 0, "%s: .mdebug section too small", dso->filename);
      return 1;
    }
  return 0;
}
