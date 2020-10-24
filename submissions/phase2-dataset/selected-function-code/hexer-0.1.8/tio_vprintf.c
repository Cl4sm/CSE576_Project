tio_vprintf(fmt, ap)
  const char *fmt;
  va_list ap;
  /* Similar to `printf()'.  `tio_printf()' understands the same @-commands
   * as `tio_display()'.  Note that @-commands in strings inserted via %s
   * are executed.  Use `tio_raw_printf()' if you don't wan't @-commands to
   * be executed.
   */
{
  char *s;
  int rval;

#if HAVE_VASPRINTF
  rval = vasprintf(&s, fmt, ap);
#else
  rval = vsnprintf(printfbuf, sizeof(printfbuf), fmt, ap);
  s = printfbuf;
#endif
  if (rval == -1)
    return rval;
  tio_display(s, 0);
#if HAVE_VASPRINTF
  free((char *)s);
#endif
  return rval;
}
