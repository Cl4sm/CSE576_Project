int main(int argc, char *argv[])
{
  int i, j, result, dashes;
  struct since_state state, *sn;
  struct sigaction sag;
  char *state_file;

  sn = &state;
  state_file = NULL;

  init_state(sn);

  i = j = 1;
  dashes = 0;

  while (i < argc) {
    if((argv[i][0] == '-') && (dashes == 0)){
      switch (argv[i][j]) {
        case 'c' :
          copying();
          return EX_OK;
        case 'h' :
          usage(argv[0]);
          return EX_OK;
          break;
        case 's' :
          j++;
          if (argv[i][j] == '\0') {
            j = 0;
            i++;
          }
          if (i >= argc) {
            fprintf(stderr, "since: -s needs a filename as parameter\n");
            return EX_USAGE;
          }
          state_file = argv[i] + j;
          i++;
          j = 1;
          break;
        case 'd':
          j++;
          if (argv[i][j] == '\0') {
            j = 0;
            i++;
          }
          if (i >= argc) {
            fprintf(stderr, "since: -d needs an integer parameter\n");
            return EX_USAGE;
          }
          sn->s_delay.tv_sec = atoi(argv[i] + j);
          sn->s_delay.tv_nsec = 0;
          if(sn->s_delay.tv_sec < 0){
            sn->s_delay.tv_sec = 1;
          }
          i++;
          j = 1;
          break;
        case 'a' :
          j++;
          sn->s_atomic = 1;
          break;
        case 'e' :
          j++;
          sn->s_header = stderr;
          break;
        case 'f' :
          j++;
          sn->s_follow = 1;
          break;
        case 'l' :
          j++;
          sn->s_relaxed = 1;
          break;
        case 'm' :
          j++;
          sn->s_domap = 1 - sn->s_domap;
          break;
        case 'n' :
          j++;
          sn->s_readonly = 1;
          break;
        case 'q' :
          j++;
          sn->s_verbose = 0;
          break;
        case 'v' :
          j++;
          sn->s_verbose++;
          break;
        case 'x' :
          j++;
          sn->s_nozip++;
          break;
        case 'z' :
          j++;
          sn->s_discard++;
          break;
#ifdef VERSION
        case 'V' :
          j++;
          printf("since %s\n", VERSION);
          return EX_OK;
#endif
        case '-' :
          if((j == 1) && argv[i][j + 1] == '\0'){
            dashes = 1;
          }
          j++;
          break;
        case '\0':
          j = 1;
          i++;
          break;
        default:
          fprintf(stderr, "since: unknown option -%c (use -h for help)\n", argv[i][j]);
          return EX_USAGE;
      }
    } else {
      result = setup_data(sn, argv[i]);
      if((result < 0) || ((sn->s_relaxed == 0) && result > 0)){
        return EX_OSERR;
      }
      i++;
    }
  }

  if(sn->s_data_count <= 0){
    fprintf(stderr, "since: need at least one filename (use -h for help)\n");
    return EX_USAGE;
  }

  /* try to open a list of files */
  if(open_state_file(sn, state_file) < 0){
    return EX_OSERR;
  }

  /* attempt to load content of said file */
  if(load_state_file(sn) < 0){
    return EX_OSERR;
  }

  /* look at content, gather size of on disk fields */
  if(check_state_file(sn) < 0){
    return EX_DATAERR;
  }

  if(maybe_upgrade_state_file(sn) < 0){
    return EX_OSERR;
  }

  if(make_fmt_string(sn) < 0){
    return EX_SOFTWARE;
  }

  if(lookup_entries(sn) < 0){
    return EX_OSERR;
  }

#ifdef DEBUG
  for(i = 0; i < sn->s_data_count; i++){
    fprintf(stderr, "dump[%d]: name=%s, fd=%d\n", i, sn->s_data_files[i].d_name, sn->s_data_files[i].d_fd);
  }
#endif

  sigemptyset(&(sn->s_set));
  sigaddset(&(sn->s_set), SIGINT);
  sigaddset(&(sn->s_set), SIGPIPE);

  sigprocmask(SIG_BLOCK, &(sn->s_set), NULL);

  sag.sa_handler = &handle_signal;
  sigfillset(&(sag.sa_mask));
  sag.sa_flags = 0;
  sigaction(SIGPIPE, &sag, NULL);
  sigaction(SIGINT, &sag, NULL);

  if(sn->s_discard){
    discard_files(sn);
  } else {
    if(display_files(sn) < 0){
      return EX_OSERR;
    }
  }
  if(sn->s_follow){
    if(setup_watch(sn) < 0){
      return EX_OSERR;
    }
    do{
      result = run_watch(sn);
      if(result == 0){
        result = display_files(sn);
      }
    } while(result == 0);
    if(result < 0){
      return EX_OSERR;
    }
  }

  if(update_state_file(sn) < 0){
    return EX_OSERR;
  }

  destroy_state(sn);

  return EX_OK;
}