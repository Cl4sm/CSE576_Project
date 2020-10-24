void readargs(int argc, char** argv, arg_t* args)
{
   args->config = NULL;
   args->btdevroot = NULL;
   args->threshold = DEFAULTTHRESHOLD;
   args->interval = DEFAULTINTERVAL;
   args->stdout = false;
   args->fork = true;
   args->verbose = false;
   args->authenticate = false;
   args->disconnecthack = false;
   args->linkquality = false;
   args->uid = 0;
   args->gid = 0;

   int c;
   static struct option long_opts[] = {
      {"config", 1, 0, 'c'},
      {"btid", 1, 0, 'b'},
      {"threshold", 1, 0, 't'},
      {"interval", 1, 0, 'i'},
      {"stdout", 0, 0, 's'},
      {"help", 0, 0, 'h'},
      {"no-fork", 0, 0, 'n'},
      {"verbose", 0, 0, 'v'},
      {"version", 0, 0, 'V'},
      {"authenticate", 0, 0, 'a'},
      {"no-syslog", 0, 0, 's'},
      {"uid", 1, 0, 'u'},
      {"gid", 1, 0, 'u'},
      {"disconnect-hack", 1, 0, 'd'},
      {"link-quality", 1, 0, 'q'}
   };
   int idx = 0;
   while (-1 != (c = getopt_long(argc, argv, "-c:b:t:i:o:sqhVndvag:u:",long_opts, &idx))) {
      switch (c) {
         case 'c':
            args->config = optarg;
            break;
         case 'b':
            args->btdevroot = btdev_append_new(args->btdevroot, optarg);
            break;
         case 't':
            args->threshold = atoi(optarg);
            break;
         case 'i':
            args->interval = atoi(optarg);
            break;
         case 'u':
            args->uid = atoi(optarg);
            break;
         case 'g':
            args->gid = atoi(optarg);
            break;
         case 's':
            args->stdout = true;
            break;
         case 'n':
            args->fork = false;
            break;
         case 'v':
            args->verbose = true;
            break;
         case 'd':
            args->disconnecthack = true;
            break;
         case 'a':
            args->authenticate = true;
            break;
         case 'q':
            args->linkquality = true;
            break;
         case 'V':
            printf("Bluemon version %s.\n", VERSION);
            break;
         case 'h':
         case ':':
         case '?':
         case 1:
            syntax();
            break;
      }
      idx = 0;
   }
}
