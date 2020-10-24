static int setup_watch(struct since_state *sn)
{
#ifdef USE_INOTIFY
  struct data_file *df;
  unsigned int i;

  sn->s_notify = inotify_init();
  if(sn->s_notify < 0){
    if(sn->s_verbose > 3){
      fprintf(stderr, "since: unable to use inotify: %s\n", strerror(errno));
    }
    return 1;
  }

  for(i = 0; i < sn->s_data_count; i++){
    df = &(sn->s_data_files[i]);
    df->d_notify = inotify_add_watch(sn->s_notify, df->d_name, IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF);
    if(df->d_notify < 0){
      fprintf(stderr, "since: unable to register notification for %s: %s\n", df->d_name, strerror(errno));
      close(sn->s_notify);
      sn->s_notify = (-1);
      return 1;
    }
  }
#endif
  return 0;
}