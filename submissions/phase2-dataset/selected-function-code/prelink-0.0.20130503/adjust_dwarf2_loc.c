static int
adjust_dwarf2_loc (DSO *dso, struct cu_data *cu, GElf_Addr offset,
		   GElf_Addr base, GElf_Addr start, GElf_Addr adjust)
{
  unsigned char *ptr, *endsec;
  GElf_Addr low, high;
  int adjusted_base;
  size_t len;

  ptr = debug_sections[DEBUG_LOC].data;
  if (ptr == NULL)
    {
      error (0, 0, "%s: loclistptr attribute, yet no .debug_loc section",
	     dso->filename);
      return 1;
    }
  if (offset >= debug_sections[DEBUG_LOC].size)
    {
      error (0, 0,
	     "%s: loclistptr offset %Ld outside of .debug_loc section",
	     dso->filename, (long long) offset);
      return 1;
    }
  endsec = ptr + debug_sections[DEBUG_LOC].size;
  ptr += offset;
  adjusted_base = (base && base >= start && addr_to_sec (dso, base) != -1);
  while (ptr < endsec)
    {
      low = read_ptr (ptr);
      high = read_ptr (ptr);
      if (low == 0 && high == 0)
	break;

      if (low == ~ (GElf_Addr) 0 || (ptr_size == 4 && low == 0xffffffff))
	{
	  base = high;
	  adjusted_base = (base && base >= start
			   && addr_to_sec (dso, base) != -1);
	  if (adjusted_base)
	    write_ptr (ptr - ptr_size, base + adjust);
	  continue;
	}
      len = read_16 (ptr);
      assert (ptr + len <= endsec);

      if (adjust_location_list (dso, cu, ptr, len, start, adjust))
	return 1;

      ptr += len;
    }

  elf_flagscn (dso->scn[debug_sections[DEBUG_LOC].sec], ELF_C_SET,
	       ELF_F_DIRTY);
  return 0;
}
