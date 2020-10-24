static unsigned char *
adjust_attributes (DSO *dso, unsigned char *ptr, struct abbrev_tag *t,
		   struct cu_data *cu,
		   GElf_Addr start, GElf_Addr adjust)
{
  int i;
  GElf_Addr addr;

  for (i = 0; i < t->nattr; ++i)
    {
      uint32_t form = t->attr[i].form;
      uint32_t len = 0;

      while (1)
	{
	  switch (t->attr[i].attr)
	    {
	    case DW_AT_data_member_location:
	      /* In DWARF4+ DW_AT_data_member_location
		 with DW_FORM_data[48] is just very high
		 constant, rather than loclistptr.  */
	      if (cu->cu_version >= 4 && form != DW_FORM_sec_offset)
		break;
	      /* FALLTHRU */
	    case DW_AT_location:
	    case DW_AT_string_length:
	    case DW_AT_return_addr:
	    case DW_AT_frame_base:
	    case DW_AT_segment:
	    case DW_AT_static_link:
	    case DW_AT_use_location:
	    case DW_AT_vtable_elem_location:
	    case DW_AT_ranges:
	      if (form == DW_FORM_data4 || form == DW_FORM_sec_offset)
		addr = read_32 (ptr), ptr -= 4;
	      else if (form == DW_FORM_data8)
		addr = read_64 (ptr), ptr -= 8;
	      else
		break;
	      {
		GElf_Addr base;

		if (cu->cu_entry_pc != ~ (GElf_Addr) 0)
		  base = cu->cu_entry_pc;
		else if (cu->cu_low_pc != ~ (GElf_Addr) 0)
		  base = cu->cu_low_pc;
	  	else
		  base = 0;
		if (t->attr[i].attr == DW_AT_ranges)
		  {
		    if (adjust_dwarf2_ranges (dso, addr, base, start, adjust))
		      return NULL;
		  }
		else
		  {
		    if (adjust_dwarf2_loc (dso, cu, addr, base, start, adjust))
		      return NULL;
		  }
	      }
	      break;
	    }
	  switch (form)
	    {
	    case DW_FORM_addr:
	      addr = read_ptr (ptr);
	      if (t->tag == DW_TAG_compile_unit
		  || t->tag == DW_TAG_partial_unit)
		{
		  if (t->attr[i].attr == DW_AT_entry_pc)
		    cu->cu_entry_pc = addr;
		  else if (t->attr[i].attr == DW_AT_low_pc)
		    cu->cu_low_pc = addr;
		  if (addr == 0)
		    break;
		}
	      if (addr >= start && addr_to_sec (dso, addr) != -1)
		write_ptr (ptr - ptr_size, addr + adjust);
	      break;
	    case DW_FORM_flag_present:
	      break;
	    case DW_FORM_ref1:
	    case DW_FORM_flag:
	    case DW_FORM_data1:
	      ++ptr;
	      break;
	    case DW_FORM_ref2:
	    case DW_FORM_data2:
	      ptr += 2;
	      break;
	    case DW_FORM_ref4:
	    case DW_FORM_GNU_ref_alt:
	    case DW_FORM_data4:
	    case DW_FORM_sec_offset:
	      ptr += 4;
	      break;
	    case DW_FORM_ref8:
	    case DW_FORM_data8:
	    case DW_FORM_ref_sig8:
	      ptr += 8;
	      break;
	    case DW_FORM_sdata:
	    case DW_FORM_ref_udata:
	    case DW_FORM_udata:
	      read_uleb128 (ptr);
	      break;
	    case DW_FORM_ref_addr:
	      if (cu->cu_version == 2)
		ptr += ptr_size;
	      else
		ptr += 4;
	      break;
	    case DW_FORM_strp:
	    case DW_FORM_GNU_strp_alt:
	      ptr += 4;
	      break;
	    case DW_FORM_string:
	      ptr = strchr (ptr, '\0') + 1;
	      break;
	    case DW_FORM_indirect:
	      form = read_uleb128 (ptr);
	      continue;
	    case DW_FORM_block1:
	      len = *ptr++;
	      break;
	    case DW_FORM_block2:
	      len = read_16 (ptr);
	      form = DW_FORM_block1;
	      break;
	    case DW_FORM_block4:
	      len = read_32 (ptr);
	      form = DW_FORM_block1;
	      break;
	    case DW_FORM_block:
	      len = read_uleb128 (ptr);
	      form = DW_FORM_block1;
	      assert (len < UINT_MAX);
	      break;
	    case DW_FORM_exprloc:
	      len = read_uleb128 (ptr);
	      assert (len < UINT_MAX);
	      break;
	    default:
	      error (0, 0, "%s: Unknown DWARF DW_FORM_%d", dso->filename,
		     form);
	      return NULL;
	    }

	  if (form == DW_FORM_block1)
	    {
	      switch (t->attr[i].attr)
		{
		case DW_AT_frame_base:
		case DW_AT_location:
		case DW_AT_data_member_location:
		case DW_AT_vtable_elem_location:
		case DW_AT_byte_size:
		case DW_AT_bit_offset:
		case DW_AT_bit_size:
		case DW_AT_string_length:
		case DW_AT_lower_bound:
		case DW_AT_return_addr:
		case DW_AT_bit_stride:
		case DW_AT_upper_bound:
		case DW_AT_count:
		case DW_AT_segment:
		case DW_AT_static_link:
		case DW_AT_use_location:
		case DW_AT_allocated:
		case DW_AT_associated:
		case DW_AT_data_location:
		case DW_AT_byte_stride:
		case DW_AT_GNU_call_site_value:
		case DW_AT_GNU_call_site_data_value:
		case DW_AT_GNU_call_site_target:
		case DW_AT_GNU_call_site_target_clobbered:
		  if (adjust_location_list (dso, cu, ptr, len, start, adjust))
		    return NULL;
		  break;
		default:
		  if (t->attr[i].attr <= DW_AT_linkage_name
		      || (t->attr[i].attr >= DW_AT_MIPS_fde
			  && t->attr[i].attr <= DW_AT_MIPS_has_inlines)
		      || (t->attr[i].attr >= DW_AT_sf_names
			  && t->attr[i].attr <= DW_AT_body_end))
		    break;
		  error (0, 0, "%s: Unknown DWARF DW_AT_%d with block DW_FORM",
			 dso->filename, t->attr[i].attr);
		  return NULL;
		}
	      ptr += len;
	    }
	  else if (form == DW_FORM_exprloc)
	    {
	      if (adjust_location_list (dso, cu, ptr, len, start, adjust))
		return NULL;
	      ptr += len;
	    }

	  break;
	}
    }

  return ptr;
}
