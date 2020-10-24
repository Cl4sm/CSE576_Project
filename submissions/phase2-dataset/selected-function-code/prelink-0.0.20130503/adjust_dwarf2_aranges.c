static int
adjust_dwarf2_aranges (DSO *dso, GElf_Addr start, GElf_Addr adjust)
{
  unsigned char *ptr = debug_sections[DEBUG_ARANGES].data;
  unsigned char *endsec = ptr + debug_sections[DEBUG_ARANGES].size;
  unsigned char *endcu;
  GElf_Addr addr, len;
  uint32_t value;

  while (ptr < endsec)
    {
      endcu = ptr + 4;
      endcu += read_32 (ptr);
      if (endcu == ptr + 0xffffffff)
	{
	  error (0, 0, "%s: 64-bit DWARF not supported", dso->filename);
	  return 1;
	}

      if (endcu > endsec)
	{
	  error (0, 0, "%s: .debug_line CU does not fit into section",
		 dso->filename);
	  return 1;
	}

      value = read_16 (ptr);
      if (value != 2)
	{
	  error (0, 0, "%s: DWARF version %d unhandled", dso->filename,
		 value);
	  return 1;
	}

      ptr += 4;
      if (ptr[0] != ptr_size || ptr[1])
	{
	  error (0, 0, "%s: Unsupported .debug_aranges address size %d or segment size %d",
		 dso->filename, ptr[0], ptr[1]);
	  return 1;
	}

      ptr += 6;
      while (ptr < endcu)
	{
	  addr = read_ptr (ptr);
	  len = read_ptr (ptr);
	  if (addr == 0 && len == 0)
	    break;
	  if (addr >= start && addr_to_sec (dso, addr) != -1)
	    write_ptr (ptr - 2 * ptr_size, addr + adjust);
	}
      assert (ptr == endcu);
    }

  elf_flagscn (dso->scn[debug_sections[DEBUG_LINE].sec], ELF_C_SET,
	       ELF_F_DIRTY);
  return 0;
}
