  static void
tio_warning_msg(const char *fmt, ...)
#else
  static void
tio_warning_msg(fmt, va_alist)
  const char *fmt;
  va_dcl
#endif
{
  va_list ap;

#if USE_STDARG
  va_start(ap, fmt);
#else
  va_start(ap);
#endif
  fprintf(stderr, "%s: warning: ", program_name);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}
