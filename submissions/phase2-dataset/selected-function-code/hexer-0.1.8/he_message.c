  void
he_message(int beep, char *fmt, ...)
#else
  void
he_message(beep, fmt, va_alist)
  int beep;
  char *fmt;
  va_dcl
#endif
{
  va_list ap;
  struct he_message_s *m;
  /* int length; */

#if USE_STDARG
  va_start(ap, fmt);
#else
  va_start(ap);
#endif
  /* length = tio_nprintf(fmt, ap); */
  m = (struct he_message_s *)malloc(sizeof(struct he_message_s));
  m->next = he_messages;
  m->beep = beep;
  /* m->message = (char *)malloc(length + 1); */
  m->message = (char *)malloc(512); /* FIXME */
  vsprintf(m->message, fmt, ap);
  he_messages = m;
  va_end(ap);
}
