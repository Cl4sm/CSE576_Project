tio_raw_vprintf(fmt, ap)
  const char *fmt;
  va_list ap;
  /* Like `printf()'.  No @-commands.
   */
{
  char *s;
  int rval;

#if HAVE_VASPRINTF
  rval = vasprintf(&s, fmt, ap);
#else
  rval = snprintf(printfbuf, sizeof(printfbuf), fmt, ap);
  s = printfbuf;
#endif
  if (rval == -1)
    return rval;
  tio_puts(s);
#if HAVE_VASPRINTF
  free((char *)s);
#endif
  return rval;
}
