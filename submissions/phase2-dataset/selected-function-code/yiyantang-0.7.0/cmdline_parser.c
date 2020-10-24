cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info)
{
  int c;	/* Character of the parsed option.  */
  int missing_required_options = 0;	

  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->from_given = 0 ;
  args_info->to_given = 0 ;
#define clear_args() { \
  args_info->from_arg = NULL; \
  args_info->to_arg = NULL; \
}

  clear_args();

  args_info->inputs = NULL;
  args_info->inputs_num = 0;

  while (1)
    {
      int option_index = 0;
      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "from",	1, NULL, 'f' },
        { "to",	1, NULL, 't' },
        { NULL,	0, NULL, 0 }
      };

      c = getopt_long (argc, argv, "hVf:t:", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          clear_args ();
          cmdline_parser_print_help ();
          exit (0);

        case 'V':	/* Print version and exit.  */
          clear_args ();
          cmdline_parser_print_version ();
          exit (0);

        case 'f':	/* Incoming encoding. Default auto-detect..  */
          if (args_info->from_given)
            {
              fprintf (stderr, "%s: `--from' (`-f') option given more than once\n", PACKAGE);
              clear_args ();
              cmdline_parser_print_help ();
              exit (1);
            }
          args_info->from_given = 1;
          args_info->from_arg = gengetopt_strdup (optarg);
          break;

        case 't':	/* Local encoding. Defaul LC_CTYPE environment..  */
          if (args_info->to_given)
            {
              fprintf (stderr, "%s: `--to' (`-t') option given more than once\n", PACKAGE);
              clear_args ();
              cmdline_parser_print_help ();
              exit (1);
            }
          args_info->to_given = 1;
          args_info->to_arg = gengetopt_strdup (optarg);
          break;

        case 0:	/* Long option with no short option */

        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          exit (1);

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c\n", PACKAGE, c);
          abort ();
        } /* switch */
    } /* while */

  if ( missing_required_options )
    exit (1);

  if (optind < argc)
    {
      int i = 0 ;

      args_info->inputs_num = argc - optind ;
      args_info->inputs = 
        (char **)(malloc ((args_info->inputs_num)*sizeof(char *))) ;
      while (optind < argc)
        args_info->inputs[ i++ ] = gengetopt_strdup (argv[optind++]) ; 
    }

  return 0;
}
