adjust_dwarf2_ranges (DSO *dso, GElf_Addr offset, GElf_Addr base,
		      GElf_Addr start, GElf_Addr adjust)
{
  unsigned char *ptr, *endsec;
  GElf_Addr low, high;
  int adjusted_base;

  ptr = debug_sections[DEBUG_RANGES].data;
  if (ptr == NULL)
    {
      error (0, 0, "%s: DW_AT_ranges attribute, yet no .debug_ranges section",
	     dso->filename);
      return 1;
    }
  if (offset >= debug_sections[DEBUG_RANGES].size)
    {
      error (0, 0,
	     "%s: DW_AT_ranges offset %Ld outside of .debug_ranges section",
	     dso->filename, (long long) offset);
      return 1;
    }
  endsec = ptr + debug_sections[DEBUG_RANGES].size;
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
	}
      else if (! adjusted_base)
	{
	  if (base + low >= start && addr_to_sec (dso, base + low) != -1)
	    {
	      write_ptr (ptr - 2 * ptr_size, low + adjust);
	      if (high == low)
		write_ptr (ptr - ptr_size, high + adjust);
	    }
	  if (low != high && base + high >= start
	      && addr_to_sec (dso, base + high - 1) != -1)
	    write_ptr (ptr - ptr_size, high + adjust);
	}
    }

  elf_flagscn (dso->scn[debug_sections[DEBUG_RANGES].sec], ELF_C_SET,
	       ELF_F_DIRTY);
  return 0;
}
