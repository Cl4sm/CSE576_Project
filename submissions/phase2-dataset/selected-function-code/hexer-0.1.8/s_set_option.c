  void
s_set_option(option, value_string, no_action)
  const char *option;
  const char *value_string;
  int no_action;
{
  struct option_s *i;
  int k;

  for (i = option_first; i; i = i->next)
    if (!strcmp(option, i->option)) break;
  assert(i);
  switch (i->type) {
    case S_STRING:
      strcpy(i->u.sval, value_string);
      if (i->action && !no_action) i->action(value_string);
      break;
    case S_INTEGER:
      i->u.ival = atol(value_string);
      if (i->action && !no_action) i->action(i->u.ival);
      break;
    case S_BOOL:
      i->u.bval = 0;
      for (k = 0; true_strings[k]; ++k)
	if (!strcasecmp(true_strings[k], value_string)) {
	  i->u.bval = 1;
	  break;
	}
      if (i->action && !no_action) i->action(i->u.bval);
      break;
    default:
      abort();
  }
}
