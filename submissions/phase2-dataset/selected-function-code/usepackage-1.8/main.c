{
   int first,i;
   char* f;
   group_t* group;
   int sh_override = -1;
   int list_packages = 0;

   for (i=1; i<argc && *argv[i] == '-'; i++)
   {
      for (f=argv[i]+1; *f; f++)
         switch (*f)
         {
            case 'v':
               debugging = 1;
               silent = 0;
               break;
            case 's':
               debugging = 0;
               silent = 1;
               break;
            case 'c':
               sh_override = 1;
               break;
            case 'b':
               sh_override = 0;
               break;
            case 'f':
               main_package_filename = argv[++i];
               break;
            case 'l':
               list_packages = 1;
               break;
            default:
               fprintf(stderr, "usepackage: unrecognised flag '%c'\n", *f);
               exit(1);
         }
   }

   if (!list_packages && (i >= argc))
   {
      fprintf(stderr, "\n%s %s, %s\n", PACKAGE_NAME, PACKAGE_VERSION, COPYRIGHT);
      fprintf(stderr, "Bug reports and comments to: %s\n", PACKAGE_BUGREPORT);
      fprintf(stderr, "Discussion/announcement list: %s\n\n", MAILING_LIST);
      fprintf(stderr, "usage: use [-vs] [-f <file>] <package> [<package>...]\n");
      fprintf(stderr, "       use -l\n\n");
      fprintf(stderr, "       -v : verbose\n");
      fprintf(stderr, "       -s : silence match warnings\n");
      fprintf(stderr, "       -f : use <file> as main packages file\n");
      fprintf(stderr, "       -l : list available packages\n\n");
      exit(1);
   }

   DEBUG(stderr, "%s\n", PACKAGE_NAME);
   DEBUG(stderr, "Version: %s\n", PACKAGE_VERSION);
   DEBUG(stderr, "%s\n", COPYRIGHT);

   uname(&the_host_info);
   DEBUG(stderr, "host: %s\n", the_host_info.nodename);
   DEBUG(stderr, "operating system: %s %s\n", the_host_info.sysname,
         the_host_info.release);
   DEBUG(stderr, "architecture: %s\n", the_host_info.machine);

   the_shell = get_user_shell();
   DEBUG(stderr, "shell: %s\n", the_shell);

   if (sh_override != -1)
      csh_user = sh_override;
   else
      csh_user = ((!strcmp(the_shell, "csh")) || (!strcmp(the_shell, "tcsh")));

   the_environment = new_list();
   the_scripts = new_list();
   
   if (get_packages(&the_packages, &the_groups, &the_annotations))
   {
      fprintf(stderr, "usepackage: couldn't load package information.\n");
      exit(2);
   }

   if (list_packages)
   {
      fprintf(stderr, "\nusepackage %s, %s\n\n", VERSION, COPYRIGHT);
      fprintf(stderr, "Available packages are:\n\n");
      list_annotations();
      fprintf(stderr, "\nAvailable groups are:\n\n");
      list_groups();
      fprintf(stderr, "\n");
      exit(0);
   }

   for (first = i, i = argc - 1 ; i >= first ; i--)
   {
      if (group = get_group(argv[i]))
         use_group(group);
      else
         use_package(argv[i]);
   }

   print_env();

   print_scripts();

   return(0);
}
