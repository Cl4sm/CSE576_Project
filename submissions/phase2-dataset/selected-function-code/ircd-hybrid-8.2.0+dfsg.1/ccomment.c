static void
ccomment(void)
{
  int c = 0;

  /* log(L_NOTICE, "got comment"); */
  while (1)
  {
    while ((c = input()) != '*' && c != EOF)
      if (c == '\n')
        ++lineno;

    if (c == '*')
    {
      while ((c = input()) == '*')
        /* Nothing */ ;
      if (c == '/')
        break;
      else if (c == '\n')
        ++lineno;
    }

    if (c == EOF)
    {
      YY_FATAL_ERROR("EOF in comment");
      /* XXX hack alert this disables
       * the stupid unused function warning
       * gcc generates
       */
      if (1 == 0)
        yy_fatal_error("EOF in comment");
      break;
    }
  }
}
