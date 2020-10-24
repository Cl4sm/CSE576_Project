static int notify_watch(struct since_state *sn)
{
#ifdef USE_INOTIFY
  struct inotify_event update;
  struct data_file *df;
  char discard[PATH_MAX];
  unsigned int i;
  int result;

#ifdef DEBUG
  if(sn->s_notify < 0){
    fprintf(stderr, "since: major logic error, no notify file descriptor\n");
    abort();
  }
#endif

  sigprocmask(SIG_UNBLOCK, &(sn->s_set), NULL);
  result = read(sn->s_notify, &update, sizeof(struct inotify_event));
  sigprocmask(SIG_BLOCK, &(sn->s_set), NULL);

  if(result < 0){
    if(since_run == 0){
      return 1;
    }
    if(sn->s_verbose > 1){
      fprintf(stderr, "since: inotify failed: %s\n", strerror(errno));
    }
    return 0;
  }

  if(result != sizeof(struct inotify_event)){
    fprintf(stderr, "since: inotify weirdness: read partial notification\n");
    return -1;
  }

  if(sn->s_verbose > 4){
    fprintf(stderr, "since: inotify mask 0x%x, len %u\n", update.mask, update.len);
  }

  if(update.len > 0){
    result = read(sn->s_notify, discard, update.len);
    if(sn->s_verbose > 3){
      fprintf(stderr, "since: discarding %u bytes of unexpected path information from inotify\n", update.len);
    }
    if(result != update.len){
      fprintf(stderr, "since: inotify weirdness: unable to discard path information\n");
      return -1;
    }
  }

  for(i = 0; i < sn->s_data_count; i++){
    df = &(sn->s_data_files[i]);
    if(update.wd == df->d_notify){

      if(check_file(sn, df) < 0){
        return -1;
      }
    }
  }

  return 0;
#else
#ifdef DEBUG
  fprintf(stderr, "since: major logic error, doing disabled notification\n");
  abort();
#endif
  return -1;
#endif
}