static int
adjust_dwarf2_info (DSO *dso, GElf_Addr start, GElf_Addr adjust, int type)
{
  unsigned char *ptr, *endcu, *endsec;
  uint32_t value;
  htab_t abbrev;
  struct abbrev_tag tag, *t;
  struct cu_data cu;

  memset (&cu, 0, sizeof(cu));
  ptr = debug_sections[type].data;
  endsec = ptr + debug_sections[type].size;
  while (ptr < endsec)
    {
      if (ptr + 11 > endsec)
	{
	  error (0, 0, "%s: .debug_info CU header too small", dso->filename);
	  return 1;
	}

      endcu = ptr + 4;
      endcu += read_32 (ptr);
      if (endcu == ptr + 0xffffffff)
	{
	  error (0, 0, "%s: 64-bit DWARF not supported", dso->filename);
	  return 1;
	}

      if (endcu > endsec)
	{
	  error (0, 0, "%s: .debug_info too small", dso->filename);
	  return 1;
	}

      value = read_16 (ptr);
      if (value != 2 && value != 3 && value != 4)
	{
	  error (0, 0, "%s: DWARF version %d unhandled", dso->filename, value);
	  return 1;
	}
      cu.cu_version = value;

      value = read_32 (ptr);
      if (value >= debug_sections[DEBUG_ABBREV].size)
	{
	  if (debug_sections[DEBUG_ABBREV].data == NULL)
	    error (0, 0, "%s: .debug_abbrev not present", dso->filename);
	  else
	    error (0, 0, "%s: DWARF CU abbrev offset too large",
		   dso->filename);
	  return 1;
	}

      if (ptr_size == 0)
	{
	  ptr_size = read_1 (ptr);
	  if (ptr_size == 4)
	    {
	      do_read_ptr = do_read_32_64;
	      write_ptr = write_32;
	    }
	  else if (ptr_size == 8)
	    {
	      do_read_ptr = do_read_64;
	      write_ptr = write_64;
	    }
	  else
	    {
	      error (0, 0, "%s: Invalid DWARF pointer size %d",
		     dso->filename, ptr_size);
	      return 1;
	    }
	}
      else if (read_1 (ptr) != ptr_size)
	{
	  error (0, 0, "%s: DWARF pointer size differs between CUs",
		 dso->filename);
	  return 1;
	}

      abbrev = read_abbrev (dso, debug_sections[DEBUG_ABBREV].data + value);
      if (abbrev == NULL)
	return 1;

      cu.cu_entry_pc = ~ (GElf_Addr) 0;
      cu.cu_low_pc = ~ (GElf_Addr) 0;

      if (type == DEBUG_TYPES)
	{
	  ptr += 8; /* Skip type_signature.  */
	  ptr += 4; /* Skip type_offset.  */
	}

      while (ptr < endcu)
	{
	  tag.entry = read_uleb128 (ptr);
	  if (tag.entry == 0)
	    continue;
	  t = htab_find_with_hash (abbrev, &tag, tag.entry);
	  if (t == NULL)
	    {
	      error (0, 0, "%s: Could not find DWARF abbreviation %d",
		     dso->filename, tag.entry);
	      htab_delete (abbrev);
	      return 1;
	    }

	  ptr = adjust_attributes (dso, ptr, t, &cu, start, adjust);
	  if (ptr == NULL)
	    {
	      htab_delete (abbrev);
	      return 1;
	    }
	}

      htab_delete (abbrev);
    }
  return 0;
}
