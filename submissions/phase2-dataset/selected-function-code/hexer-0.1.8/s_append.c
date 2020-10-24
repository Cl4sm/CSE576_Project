  static void
s_append(struct option_s *last,
         const char *option,
         const enum s_option_e type, ...)
#else
  static void
s_append(last, option, type, va_alist)
  struct option_s *last;
  const char *option;
  const enum s_option_e type; 
  va_dcl
#endif
{
  va_list ap;
  struct option_s *i;

#if USE_STDARG
  va_start(ap, type);
#else
  va_start(ap);
#endif
  i = (struct option_s *)malloc(sizeof(struct option_s));
  i->type = type;
  i->next = 0;
  strcpy(i->option, option);
  if (last) last->next = i; else option_first = i;
  i->action = 0;
  switch (type) {
    case S_STRING:
      strcpy(i->u.sval, va_arg(ap, char *));
      break;
    case S_INTEGER:
      i->u.ival = va_arg(ap, long);
      break;
    case S_BOOL:
      i->u.bval = !!va_arg(ap, int);
      break;
    default:
      abort();
  }
  va_end(ap);
}
