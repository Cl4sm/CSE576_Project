int parse_options (const char *const program_name, const int argc, char **const argv)
{
  static const char *const optstr__blocks_file = "blocks-file";
  static const char *const optstr__method = "method";
  static const char *const optstr__verbose = "verbose";
  static const char *const optstr__info = "info";
  static const char *const optstr__strip = "strip";
  static const char *const optstr__help = "help";
  static const char *const optstr__version = "version";
  int i = 0;
  opt_blocks_file = 0;
  opt_method = 0;
  opt_verbose = 0;
  opt_info = 0;
  opt_strip = 0;
  opt_help = 0;
  opt_version = 0;
  arg_blocks_file = 0;
  arg_method = 0;
  while (++i < argc)
  {
    const char *option = argv [i];
    if (*option != '-')
      return i;
    else if (*++option == '\0')
      return i;
    else if (*option == '-')
    {
      const char *argument;
      size_t option_len;
      ++option;
      if ((argument = strchr (option, '=')) == option)
        goto error_unknown_long_opt;
      else if (argument == 0)
        option_len = strlen (option);
      else
        option_len = argument++ - option;
      switch (*option)
      {
       case '\0':
        return i + 1;
       case 'b':
        if (strncmp (option + 1, optstr__blocks_file + 1, option_len - 1) == 0)
        {
          if (argument != 0)
            arg_blocks_file = argument;
          else if (++i < argc)
            arg_blocks_file = argv [i];
          else
          {
            option = optstr__blocks_file;
            goto error_missing_arg_long;
          }
          opt_blocks_file = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'h':
        if (strncmp (option + 1, optstr__help + 1, option_len - 1) == 0)
        {
          if (argument != 0)
          {
            option = optstr__help;
            goto error_unexpec_arg_long;
          }
          opt_help = 1;
          return i + 1;
        }
        goto error_unknown_long_opt;
       case 'i':
        if (strncmp (option + 1, optstr__info + 1, option_len - 1) == 0)
        {
          if (argument != 0)
          {
            option = optstr__info;
            goto error_unexpec_arg_long;
          }
          opt_info = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'm':
        if (strncmp (option + 1, optstr__method + 1, option_len - 1) == 0)
        {
          if (argument != 0)
            arg_method = argument;
          else if (++i < argc)
            arg_method = argv [i];
          else
          {
            option = optstr__method;
            goto error_missing_arg_long;
          }
          opt_method = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 's':
        if (strncmp (option + 1, optstr__strip + 1, option_len - 1) == 0)
        {
          if (argument != 0)
          {
            option = optstr__strip;
            goto error_unexpec_arg_long;
          }
          opt_strip = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'v':
        if (strncmp (option + 1, optstr__verbose + 1, option_len - 1) == 0)
        {
          if (option_len <= 3)
            goto error_long_opt_ambiguous;
          if (argument != 0)
          {
            option = optstr__verbose;
            goto error_unexpec_arg_long;
          }
          opt_verbose = 1;
          break;
        }
        if (strncmp (option + 1, optstr__version + 1, option_len - 1) == 0)
        {
          if (option_len <= 3)
            goto error_long_opt_ambiguous;
          if (argument != 0)
          {
            option = optstr__version;
            goto error_unexpec_arg_long;
          }
          opt_version = 1;
          return i + 1;
        }
       default:
       error_unknown_long_opt:
        fprintf (stderr, STR_ERR_UNKNOWN_LONG_OPT, program_name, option);
        return -1;
       error_long_opt_ambiguous:
        fprintf (stderr, STR_ERR_LONG_OPT_AMBIGUOUS, program_name, option);
        return -1;
       error_missing_arg_long:
        fprintf (stderr, STR_ERR_MISSING_ARG_LONG, program_name, option);
        return -1;
       error_unexpec_arg_long:
        fprintf (stderr, STR_ERR_UNEXPEC_ARG_LONG, program_name, option);
        return -1;
      }
    }
    else
      do
      {
        switch (*option)
        {
         case 'f':
          if (option [1] != '\0')
            arg_blocks_file = option + 1;
          else if (++i < argc)
            arg_blocks_file = argv [i];
          else
            goto error_missing_arg_short;
          option = "\0";
          opt_blocks_file = 1;
          break;
         case 'i':
          opt_info = 1;
          break;
         case 'm':
          if (option [1] != '\0')
            arg_method = option + 1;
          else if (++i < argc)
            arg_method = argv [i];
          else
            goto error_missing_arg_short;
          option = "\0";
          opt_method = 1;
          break;
         case 's':
          opt_strip = 1;
          break;
         case 'v':
          opt_verbose = 1;
          break;
         default:
          fprintf (stderr, STR_ERR_UNKNOWN_SHORT_OPT, program_name, *option);
          return -1;
         error_missing_arg_short:
          fprintf (stderr, STR_ERR_MISSING_ARG_SHORT, program_name, *option);
          return -1;
        }
      } while (*++option != '\0');
  }
  return i;
}