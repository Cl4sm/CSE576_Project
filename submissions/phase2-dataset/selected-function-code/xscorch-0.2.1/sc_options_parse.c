
  #if USE_NETWORK
   char name[SC_OPTION_BUFFER];
   char server[SC_OPTION_BUFFER];
   bool isserver = false;
   bool isclient = false;
   int port;
  #endif /* Net? */
   int ch;
   char digest[16];

   /* Waif's Hidden Easter Egg =)  */
   unsigned char whee[16]  = { 0x0f, 0x6f, 0xfd, 0xa3, 0xc9, 0xc9, 0xce, 0x15,
                               0xf2, 0x79, 0xb7, 0x88, 0xb4, 0x86, 0xe9, 0xca };
   /* Justin's response to Waif's Hidden Easter Egg :)  */
   unsigned char wheee[16] = { 0x22, 0x47, 0x0e, 0x3a, 0x15, 0x8a, 0x76, 0x86,
                               0xe1, 0xf7, 0x5c, 0xd9, 0xb8, 0x14, 0x52, 0x3a };

   /* Set network default options */
   #if USE_NETWORK
      strcopyb(name, getenv("USER"), sizeof(name));
      strcopyb(server, SC_NET_DEFAULT_SERVER, sizeof(name));
      port = SC_NET_DEFAULT_PORT;
   #endif /* Net? */

   /* Set up getopt.  We turn off the "unknown option" warning for the sake
      of hidden options.  We keep track of the option parsing so we can look
      hidden options and print a warning if they're really missing options. */
   opterr = optind = optopt = 0;

   while(EOF != (ch = getopt_long(argc, argv, "g:sSh", _sc_long_options, NULL))) switch(ch) {
      case SC_OPTION_CONFIG_FILE:
         /* New config file to parse in! */
         reg_set_name(c->cfreg, optarg);
         if(!sc_config_file_load(c)) {
            fprintf(stderr, "Failed to load alternate configuration file \"%s\".\n", optarg);
         }
         break;

      case SC_OPTION_INSANITY:
         /* If you are reading this, then umm... wow. */
         /* You'll find no help here, why do you want to know? */
         /* This is where mechanical excellence, one-thousand
            four-hundred horsepower pays off.  */
         fprintf(stderr, "Warning:  The insanity subsystem is enabled.\n\n");
         fprintf(stderr, "   Life... Dreams... Hope...\n");
         fprintf(stderr, "   Where'd they come from...\n");
         fprintf(stderr, "   And where are they headed?\n");
         fprintf(stderr, "   These things... I am going to destroy!\n");
         fprintf(stderr, "    -- Kefka, Fourth Tier, Final Fantasy VI\n\n");

         c->insanity = true;
         break;

      case SC_OPTION_YIELDS:
         sc_weapon_print_yields(c->weapons);
         return(1);

      case 'g':
         if(_sc_options_x_y(optarg, &c->fieldwidth, &c->fieldheight)) {
            if(c->fieldwidth < SC_MIN_FIELD_WIDTH || c->fieldwidth > SC_MAX_FIELD_WIDTH) {
               fprintf(stderr, "--geometry, width given, %d, out of range [%d, %d]\n",
                       c->fieldwidth, SC_MIN_FIELD_WIDTH, SC_MAX_FIELD_WIDTH);
               return(1);
            } else if(c->fieldheight < SC_MIN_FIELD_HEIGHT || c->fieldheight > SC_MAX_FIELD_HEIGHT) {
               fprintf(stderr, "--geometry, height given, %d, out of range [%d, %d]\n",
                       c->fieldheight, SC_MIN_FIELD_HEIGHT, SC_MAX_FIELD_HEIGHT);
               return(1);
            }
            if(c->fieldheight < c->maxheight) {
               c->maxheight = c->fieldheight - 32;
            }
            sc_land_setup(c->land, c->fieldwidth, c->fieldheight, sc_land_flags(c));
         } else {
            fprintf(stderr, "--geometry requires an option of form <width>x<height>, e.g. \"640x480\"\n");
            return(1);
         }
         break;

      case 'h':
         return(sc_options_usage(argv[0]));

      case 'S':
         c->enablesound = true;
         break;

      case 's':
         c->enablesound = false;
         break;

      case SC_OPTION_HQMIXER:
         c->usehqmixer = true;
         break;

      case SC_OPTION_NO_HQMIXER:
         c->usehqmixer = false;
         break;

      #if USE_NETWORK
         case SC_OPTION_NAME:
            strcopyb(name, optarg, sizeof(name));
            break;
         case SC_OPTION_PORT:
            port = atoi(optarg);
            break;
         case SC_OPTION_SERVER:
            isserver = true;
            break;
         case SC_OPTION_CLIENT:
            isclient = true;
            strcopyb(server, optarg, sizeof(server));
            break;
      #endif /* Network? */

      case '?':
         /* Unknown option. Hidden? */
         if(0 == optopt) {
            /* Unknown, starts with -- */
            md5_buffer(argv[optind - 1] + 2, strlenn(argv[optind - 1]) - 2, (void *)digest);

            if(!memcmp((void *)whee, (void *)digest, 16)) {
               printf("WHEE! You've found Waif's Hidden Easter Egg!\n");
               printf("   ...which is still in progress =/\n\n");
               printf("JDS:  I *finally* figured out what this bloody encrypted string is.\n");
               printf("      Could it be, perhaps, that Waif is dropping a subtle hint here?\n\n");
               printf("JDS:  So, since you found this, I'll give a little story.  A long time\n"
                      "      ago, YEARS ago probably, I figured out what this option was.  If\n"
                      "      you're reading this from the console output then you obviously\n"
                      "      know what it is too.  Anywho, at some point the code broke and\n"
                      "      the check was negated, allowing almost any option to \"match\"\n"
                      "      except for the correct one.\n\n"
                      "      Well, one day I tried \"--version\" and triggered the egg due to\n"
                      "      the bug.  My above comment was in response to triggering that\n"
                      "      bug, and thinking that waif was perhaps dropping a hint for a new\n"
                      "      option we needed to support.  I am clearly mistaken :)  Still, it\n"
                      "      makes for an odd story.\n\n"
                      "      At any rate, my response to waif's option is the next line down :)\n");
               break;
            }

            if(!memcmp((void *)wheee, (void *)digest, 16)) {
               printf("Wow, you found Justin's response to waif's egg!  Impressive...\n\n");
               break;
            }

            /* really unknown */
            fprintf(stderr, "%s: unrecognized option `%s'\n", argv[0], argv[optind-1]);
         } else {
            /* really unknown, single - type option */
            fprintf(stderr, "%s: unrecognized option `%c'\n", argv[0], optopt);
         }

         /* FALLTHROUGH OK */

      case ':':
      default:
         return(sc_options_usage(argv[0]));

   }

   /* Running a server or client connection? */
   #if USE_NETWORK
      if(isserver) {
         c->server = sc_net_server_new(c, port);
         if(c->server != NULL) isclient = true;
      }
      if(isclient) {
         c->client = sc_net_client_new(name, server, port);
      }
   #endif /* Network? */

   return(0);

}
