static int
adjust_dwarf2_line (DSO *dso, GElf_Addr start, GElf_Addr adjust)
{
  unsigned char *ptr = debug_sections[DEBUG_LINE].data;
  unsigned char *endsec = ptr + debug_sections[DEBUG_LINE].size;
  unsigned char *endcu, *endprol;
  unsigned char opcode_base, *opcode_lengths, op;
  uint32_t value;
  GElf_Addr addr;
  int i;

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
      if (value != 2 && value != 3 && value != 4)
	{
	  error (0, 0, "%s: DWARF version %d unhandled", dso->filename,
		 value);
	  return 1;
	}

      endprol = ptr + 4;
      endprol += read_32 (ptr);
      if (endprol > endcu)
	{
	  error (0, 0, "%s: .debug_line CU prologue does not fit into CU",
		 dso->filename);
	  return 1;
	}

      opcode_base = ptr[4 + (value >= 4)];
      opcode_lengths = ptr + 4 + (value >= 4);

      ptr = endprol;
      while (ptr < endcu)
	{
	  op = *ptr++;
	  if (op >= opcode_base)
	    continue;
	  if (op == DW_LNS_extended_op)
	    {
	      unsigned int len = read_uleb128 (ptr);

	      assert (len < UINT_MAX);
	      op = *ptr++;
	      switch (op)
		{
		case DW_LNE_set_address:
		  addr = read_ptr (ptr);
		  if (addr >= start && addr_to_sec (dso, addr) != -1)
		    write_ptr (ptr - ptr_size, addr + adjust);
		  break;
		case DW_LNE_end_sequence:
		case DW_LNE_define_file:
		case DW_LNE_set_discriminator:
		default:
		  ptr += len - 1;
		  break;
		}
	    }
	  else if (op == DW_LNS_fixed_advance_pc)
	    ptr += 2;
	  else
	    for (i = 0; i < opcode_lengths[op]; ++i)
	      read_uleb128 (ptr);
	}
    }

  elf_flagscn (dso->scn[debug_sections[DEBUG_LINE].sec], ELF_C_SET,
	       ELF_F_DIRTY);
  return 0;
}
