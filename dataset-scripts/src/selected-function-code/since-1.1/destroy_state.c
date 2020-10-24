void destroy_state(struct since_state *sn)
{
  int i;
  struct data_file *df;

  forget_state_file(sn);

  if(sn->s_data_files){
    for(i = 0; i < sn->s_data_count; i++){
      df = &(sn->s_data_files[i]);
      if(df->d_fd >= 0){
        close(df->d_fd);
        df->d_fd = (-1);
      }
      df->d_offset = (-1);
    }
    free(sn->s_data_files);
    sn->s_data_files = NULL;
  }
  sn->s_data_count = 0;

  if(sn->s_notify >= 0){
    close(sn->s_notify);
    sn->s_notify = (-1);
  }

  if(sn->s_fd >= 0){
    close(sn->s_fd);
    sn->s_fd = (-1);
  }

  if(sn->s_name){
    free(sn->s_name);
    sn->s_name = NULL;
  }
}