  static int
tio_command(char *cmd, int affcnt, ...)
#else
  static int
tio_command(cmd, affcnt, va_alist)
  char *cmd;
  int affcnt;
  va_dcl
#endif
  /* send the command `cmd' to the terminal.
   * return value: 0 if all goes well and -1 on error.
   */
{
  va_list ap;
  int arg1, arg2;

  if (!cmd) return -1;
#if USE_STDARG
  va_start(ap, affcnt);
#else
  va_start(ap);
#endif
  arg1 = va_arg(ap, int);
  arg2 = va_arg(ap, int);
  tputs(tgoto(cmd, arg2, arg1), affcnt, outc);
  return 0;
}
