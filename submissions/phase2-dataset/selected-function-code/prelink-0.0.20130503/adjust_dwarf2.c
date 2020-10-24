int
adjust_dwarf2 (DSO *dso, int n, GElf_Addr start, GElf_Addr adjust)
{
  Elf_Data *data;
  Elf_Scn *scn;
  int i, j;

  for (i = 0; debug_sections[i].name; ++i)
    {
      debug_sections[i].data = NULL;
      debug_sections[i].size = 0;
      debug_sections[i].sec = 0;
    }
  ptr_size = 0;

  for (i = 1; i < dso->ehdr.e_shnum; ++i)
    if (! (dso->shdr[i].sh_flags & (SHF_ALLOC | SHF_WRITE | SHF_EXECINSTR))
	&& dso->shdr[i].sh_size)
      {
	const char *name = strptr (dso, dso->ehdr.e_shstrndx,
				   dso->shdr[i].sh_name);

	if (strncmp (name, ".debug_", sizeof (".debug_") - 1) == 0)
	  {
	    for (j = 0; debug_sections[j].name; ++j)
	      if (strcmp (name, debug_sections[j].name) == 0)
	 	{
		  if (debug_sections[j].data)
		    {
		      error (0, 0, "%s: Found two copies of %s section",
			     dso->filename, name);
		      return 1;
		    }

		  scn = dso->scn[i];
		  data = elf_getdata (scn, NULL);
		  assert (data != NULL && data->d_buf != NULL);
		  assert (elf_getdata (scn, data) == NULL);
		  assert (data->d_off == 0);
		  assert (data->d_size == dso->shdr[i].sh_size);
		  debug_sections[j].data = data->d_buf;
		  debug_sections[j].size = data->d_size;
		  debug_sections[j].sec = i;
		  break;
		}

	    if (debug_sections[j].name == NULL)
	      {
		error (0, 0, "%s: Unknown debugging section %s",
		       dso->filename, name);
		return 1;
	      }
	  }
      }

  if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2LSB)
    {
      do_read_16 = buf_read_ule16;
      do_read_32 = buf_read_ule32;
      do_read_32_64 = buf_read_ule32_64;
      do_read_64 = buf_read_ule64;
      write_32 = dwarf2_write_le32;
      write_64 = dwarf2_write_le64;
    }
  else if (dso->ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
    {
      do_read_16 = buf_read_ube16;
      do_read_32 = buf_read_ube32;
      do_read_32_64 = buf_read_ube32_64;
      do_read_64 = buf_read_ube64;
      write_32 = dwarf2_write_be32;
      write_64 = dwarf2_write_be64;
    }
  else
    {
      error (0, 0, "%s: Wrong ELF data enconding", dso->filename);
      return 1;
    }

  if (debug_sections[DEBUG_INFO].data != NULL
      && adjust_dwarf2_info (dso, start, adjust, DEBUG_INFO))
    return 1;

  if (debug_sections[DEBUG_TYPES].data != NULL
      && adjust_dwarf2_info (dso, start, adjust, DEBUG_TYPES))
    return 1;

  if (ptr_size == 0)
    /* Should not happen.  */
    ptr_size = dso->ehdr.e_ident[EI_CLASS] == ELFCLASS64 ? 8 : 4;

  if (debug_sections[DEBUG_LINE].data != NULL
      && adjust_dwarf2_line (dso, start, adjust))
    return 1;

  if (debug_sections[DEBUG_ARANGES].data != NULL
      && adjust_dwarf2_aranges (dso, start, adjust))
    return 1;

  if (debug_sections[DEBUG_FRAME].data != NULL
      && adjust_dwarf2_frame (dso, start, adjust))
    return 1;

  /* .debug_abbrev requires no adjustement.  */
  /* .debug_pubnames requires no adjustement.  */
  /* .debug_pubtypes requires no adjustement.  */
  /* .debug_macinfo requires no adjustement.  */
  /* .debug_str requires no adjustement.  */
  /* .debug_ranges adjusted for each DW_AT_ranges pointing into it.  */
  /* .debug_loc adjusted for each loclistptr pointing into it.  */

  elf_flagscn (dso->scn[n], ELF_C_SET, ELF_F_DIRTY);
  return 0;
}
