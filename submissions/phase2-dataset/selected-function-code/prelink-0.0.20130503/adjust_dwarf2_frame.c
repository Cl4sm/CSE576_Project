static int
adjust_dwarf2_frame (DSO *dso, GElf_Addr start, GElf_Addr adjust)
{
  unsigned char *ptr = debug_sections[DEBUG_FRAME].data;
  unsigned char *endsec = ptr + debug_sections[DEBUG_FRAME].size;
  unsigned char *endie;
  GElf_Addr addr, len;
  uint32_t value;

  while (ptr < endsec)
    {
      endie = ptr + 4;
      endie += read_32 (ptr);
      if (endie == ptr + 0xffffffff)
	{
	  error (0, 0, "%s: 64-bit DWARF not supported", dso->filename);
	  return 1;
	}

      if (endie > endsec)
	{
	  error (0, 0, "%s: .debug_frame CIE/FDE does not fit into section",
		 dso->filename);
	  return 1;
	}

      value = read_32 (ptr);
      if (value == 0xffffffff)
	{
	  /* CIE.  */
	  uint32_t version = *ptr++;
	  if (version != 1 && version != 3 && version != 4)
	    {
	      error (0, 0, "%s: unhandled .debug_frame version %d",
		     dso->filename, version);
	      return 1;
	    }
	  if (*ptr != '\0')
	    {
	      error (0, 0, "%s: .debug_frame unhandled augmentation \"%s\"",
		     dso->filename, ptr);
	      return 1;
	    }
	  ptr++;  /* Skip augmentation.  */
	  if (version >= 4)
	    {
	      if (ptr[0] != ptr_size)
		{
		  error (0, 0, "%s: .debug_frame unhandled pointer size %d",
			  dso->filename, ptr[0]);
		  return 1;
		}
	      if (ptr[1] != 0)
		{
		  error (0, 0, "%s: .debug_frame unhandled non-zero segment size",
			 dso->filename);
		  return 1;
		}
	      ptr += 2;
	    }
	  read_uleb128 (ptr);  /* Skip code_alignment factor.  */
	  read_uleb128 (ptr);  /* Skip data_alignment factor.  */
	  if (version >= 3)
	    read_uleb128 (ptr);  /* Skip return_address_register.  */
	  else
	    ptr++;
	}
      else
	{
	  addr = read_ptr (ptr);
	  if (addr >= start && addr_to_sec (dso, addr) != -1)
	    write_ptr (ptr - ptr_size, addr + adjust);
	  read_ptr (ptr);  /* Skip address range.  */
	}

      while (ptr < endie)
	{
	  unsigned char insn = *ptr++;

	  if ((insn & 0xc0) == DW_CFA_advance_loc
	      || (insn & 0xc0) == DW_CFA_restore)
	    continue;
	  else if ((insn & 0xc0) == DW_CFA_offset)
	    {
	      read_uleb128 (ptr);
	      continue;
	    }
	  switch (insn)
	    {
	    case DW_CFA_nop:
	    case DW_CFA_remember_state:
	    case DW_CFA_restore_state:
	    case DW_CFA_GNU_window_save:
	      break;
	    case DW_CFA_offset_extended:
	    case DW_CFA_register:
	    case DW_CFA_def_cfa:
	    case DW_CFA_offset_extended_sf:
	    case DW_CFA_def_cfa_sf:
	    case DW_CFA_GNU_negative_offset_extended:
	    case DW_CFA_val_offset:
	    case DW_CFA_val_offset_sf:
	      read_uleb128 (ptr);
	      /* FALLTHROUGH */
	    case DW_CFA_restore_extended:
	    case DW_CFA_undefined:
	    case DW_CFA_same_value:
	    case DW_CFA_def_cfa_register:
	    case DW_CFA_def_cfa_offset:
	    case DW_CFA_def_cfa_offset_sf:
	    case DW_CFA_GNU_args_size:
	      read_uleb128 (ptr);
	      break;
	    case DW_CFA_set_loc:
	      addr = read_ptr (ptr);
	      if (addr >= start && addr_to_sec (dso, addr) != -1)
		write_ptr (ptr - ptr_size, addr + adjust);
	      break;
	    case DW_CFA_advance_loc1:
	      ptr++;
	      break;
	    case DW_CFA_advance_loc2:
	      ptr += 2;
	      break;
	    case DW_CFA_advance_loc4:
	      ptr += 4;
	      break;
	    case DW_CFA_expression:
	    case DW_CFA_val_expression:
	      read_uleb128 (ptr);
	      /* FALLTHROUGH */
	    case DW_CFA_def_cfa_expression:
	      len = read_uleb128 (ptr);
	      if (adjust_location_list (dso, NULL, ptr, len, start, adjust))
		return 1;
	      ptr += len;
	      break;
	    default:
	      error (0, 0, "%s: Unhandled DW_CFA_%02x operation",
		     dso->filename, insn);
	      return 1;
	    }
	}
    }

  elf_flagscn (dso->scn[debug_sections[DEBUG_FRAME].sec], ELF_C_SET,
	       ELF_F_DIRTY);
  return 0;
}
