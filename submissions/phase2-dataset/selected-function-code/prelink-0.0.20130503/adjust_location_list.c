adjust_location_list (DSO *dso, struct cu_data *cu, unsigned char *ptr,
		      size_t len, GElf_Addr start, GElf_Addr adjust)
{
  unsigned char *end = ptr + len;
  unsigned char op;
  GElf_Addr addr;

  while (ptr < end)
    {
      op = *ptr++;
      switch (op)
	{
	case DW_OP_addr:
	  addr = read_ptr (ptr);
	  if (addr >= start && addr_to_sec (dso, addr) != -1)
	    write_ptr (ptr - ptr_size, addr + adjust);
	  break;
	case DW_OP_deref:
	case DW_OP_dup:
	case DW_OP_drop:
	case DW_OP_over:
	case DW_OP_swap:
	case DW_OP_rot:
	case DW_OP_xderef:
	case DW_OP_abs:
	case DW_OP_and:
	case DW_OP_div:
	case DW_OP_minus:
	case DW_OP_mod:
	case DW_OP_mul:
	case DW_OP_neg:
	case DW_OP_not:
	case DW_OP_or:
	case DW_OP_plus:
	case DW_OP_shl:
	case DW_OP_shr:
	case DW_OP_shra:
	case DW_OP_xor:
	case DW_OP_eq:
	case DW_OP_ge:
	case DW_OP_gt:
	case DW_OP_le:
	case DW_OP_lt:
	case DW_OP_ne:
	case DW_OP_lit0 ... DW_OP_lit31:
	case DW_OP_reg0 ... DW_OP_reg31:
	case DW_OP_nop:
	case DW_OP_push_object_address:
	case DW_OP_form_tls_address:
	case DW_OP_call_frame_cfa:
	case DW_OP_stack_value:
	case DW_OP_GNU_push_tls_address:
	case DW_OP_GNU_uninit:
	  break;
	case DW_OP_const1u:
	case DW_OP_pick:
	case DW_OP_deref_size:
	case DW_OP_xderef_size:
	case DW_OP_const1s:
	  ++ptr;
	  break;
	case DW_OP_const2u:
	case DW_OP_const2s:
	case DW_OP_skip:
	case DW_OP_bra:
	case DW_OP_call2:
	  ptr += 2;
	  break;
	case DW_OP_const4u:
	case DW_OP_const4s:
	case DW_OP_call4:
	case DW_OP_GNU_parameter_ref:
	  ptr += 4;
	  break;
	case DW_OP_call_ref:
	  if (cu == NULL)
	    {
	      error (0, 0, "%s: DWARF DW_OP_call_ref shouldn't appear"
		     " in .debug_frame", dso->filename);
	      return 1;
	    }
	  if (cu->cu_version == 2)
	    ptr += ptr_size;
	  else
	    ptr += 4;
	  break;
	case DW_OP_const8u:
	case DW_OP_const8s:
	  ptr += 8;
	  break;
	case DW_OP_constu:
	case DW_OP_plus_uconst:
	case DW_OP_regx:
	case DW_OP_piece:
	case DW_OP_consts:
	case DW_OP_breg0 ... DW_OP_breg31:
	case DW_OP_fbreg:
	case DW_OP_GNU_convert:
	case DW_OP_GNU_reinterpret:
	  read_uleb128 (ptr);
	  break;
	case DW_OP_bregx:
	case DW_OP_bit_piece:
	case DW_OP_GNU_regval_type:
	  read_uleb128 (ptr);
	  read_uleb128 (ptr);
	  break;
	case DW_OP_implicit_value:
	  {
	    uint32_t leni = read_uleb128 (ptr);
	    ptr += leni;
	  }
	  break;
	case DW_OP_GNU_implicit_pointer:
	  if (cu == NULL)
	    {
	      error (0, 0, "%s: DWARF DW_OP_GNU_implicit_pointer shouldn't"
		     " appear in .debug_frame", dso->filename);
	      return 1;
	    }
	  if (cu->cu_version == 2)
	    ptr += ptr_size;
	  else
	    ptr += 4;
	  read_uleb128 (ptr);
	  break;
        case DW_OP_GNU_entry_value:
	  {
	    uint32_t leni = read_uleb128 (ptr);
	    if ((end - ptr) < leni)
	      {
		error (0, 0, "%s: DWARF DW_OP_GNU_entry_value with too large"
		       " length", dso->filename);
		return 1;
	      }
	    if (adjust_location_list (dso, cu, ptr, leni, start, adjust))
	      return 1;
	    ptr += leni;
	  }
	  break;
        case DW_OP_GNU_const_type:
	  read_uleb128 (ptr);
	  ptr += *ptr + 1;
	  break;
	case DW_OP_GNU_deref_type:
	  ++ptr;
	  read_uleb128 (ptr);
	  break;
	default:
	  error (0, 0, "%s: Unknown DWARF DW_OP_%d", dso->filename, op);
	  return 1;
	}
    }
  return 0;
}
