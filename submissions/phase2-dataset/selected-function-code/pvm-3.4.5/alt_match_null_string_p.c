static boolean
alt_match_null_string_p (p, end, reg_info)
    unsigned char *p, *end;
    register_info_type *reg_info;
{
  int mcnt;
  unsigned char *p1 = p;
  
  while (p1 < end)
    {
      /* Skip over opcodes that can match nothing, and break when we get 
         to one that can't.  */
      
      switch ((re_opcode_t) *p1)
        {
	/* It's a loop.  */
        case on_failure_jump:
          p1++;
          EXTRACT_NUMBER_AND_INCR (mcnt, p1);
          p1 += mcnt;
          break;
          
	default: 
          if (!common_op_match_null_string_p (&p1, end, reg_info))
            return false;
        }
    }  /* while p1 < end */

  return true;
} /* alt_match_null_string_p */
