re_compile_pattern (pattern, length, bufp)
     const char *pattern;
     int length;
     struct re_pattern_buffer *bufp;
{
  reg_errcode_t ret;
  
  /* GNU code is written to assume at least RE_NREGS registers will be set
     (and at least one extra will be -1).  */
  bufp->regs_allocated = REGS_UNALLOCATED;
  
  /* And GNU code determines whether or not to get register information
     by passing null for the REGS argument to re_match, etc., not by
     setting no_sub.  */
  bufp->no_sub = 0;
  
  /* Match anchors at newline.  */
  bufp->newline_anchor = 1;
  
  ret = regex_compile (pattern, length, re_syntax_options, bufp);

  return re_error_msg[(int) ret];
}     
