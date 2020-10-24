static int poll_watch(struct since_state *sn)
{
  unsigned int i;
  struct data_file *df;

  sigprocmask(SIG_UNBLOCK, &(sn->s_set), NULL);
  nanosleep(&(sn->s_delay), NULL);
  sigprocmask(SIG_BLOCK, &(sn->s_set), NULL);

  if(since_run == 0){
    return 1;
  }

  for(i = 0; i < sn->s_data_count; i++){
    df = &(sn->s_data_files[i]);
    if(check_file(sn, df) < 0){
      return -1;
    }
  }

  return 0;
}