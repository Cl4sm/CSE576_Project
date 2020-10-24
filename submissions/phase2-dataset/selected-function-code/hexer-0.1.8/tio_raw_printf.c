tio_raw_printf(const char *fmt, ...)
#else
  int
tio_raw_printf(fmt, va_alist)
  const char *fmt;
  va_dcl
#endif
{
  va_list ap;
  int rval;

#if USE_STDARG
  va_start(ap, fmt);
#else
  va_start(ap);
#endif
  rval = tio_raw_vprintf(fmt, ap);
  va_end(ap);
  return rval;
}
