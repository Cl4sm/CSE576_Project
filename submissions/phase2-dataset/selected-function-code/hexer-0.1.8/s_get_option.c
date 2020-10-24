s_get_option(option)
  const char *option;
{
  struct option_s *i;
  static char rval[1024];

  for (i = option_first; i; i = i->next)
    if (!strcmp(option, i->option)) break;
  if (!i) return "";
  switch (i->type) {
    case S_STRING:
      strcpy(rval, i->u.sval);
      break;
    case S_INTEGER:
      sprintf(rval, "%li", i->u.ival);
      break;
    case S_BOOL:
      if (i->u.bval)
	strcpy(rval, true_strings[0]);
      else
	strcpy(rval, false_string);
      break;
    default:
      abort();
  }
  return rval;
}
