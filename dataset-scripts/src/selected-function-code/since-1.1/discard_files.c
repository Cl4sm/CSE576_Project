static void discard_files(struct since_state *sn)
{
  unsigned int i;
  struct data_file *df;
  int single;

#ifdef DEBUG
  fprintf(stderr, "display: have %u files to display\n", sn->s_data_count);
#endif

  single = (sn->s_data_count == 1)  ? 1 : 0;

  for(i = 0; i < sn->s_data_count; i++){
    df = &(sn->s_data_files[i]);

    display_header(sn, df, single, 1);

    if(df->d_pos != df->d_now){
      df->d_pos = df->d_now;
      df->d_jump = 1;
      df->d_write = 1;
    }
  }

}