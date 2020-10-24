static int
ieof(void)
{
  /* log(L_NOTICE, "return from include stack!"); */
  if (include_stack_ptr)
    fclose(conf_parser_ctx.conf_file);
  if (--include_stack_ptr < 0)
  {
    /* log(L_NOTICE, "terminating lexer"); */
    /* We will now exit the lexer - restore init values if we get /rehash
     * later and reenter lexer -kre */
    include_stack_ptr = 0;
    lineno = 1;
    return 1;
  }

  /* switch buffer */
  /* log(L_NOTICE, "deleting include_stack_ptr=%d", include_stack_ptr); */
  yy_delete_buffer(YY_CURRENT_BUFFER);
  lineno = lineno_stack[include_stack_ptr];
  conf_parser_ctx.conf_file = inc_fbfile_in[include_stack_ptr];
  strlcpy(conffilebuf, conffile_stack[include_stack_ptr], sizeof(conffilebuf));
  yy_switch_to_buffer(include_stack[include_stack_ptr]);

  return 0;
}
