static int check_file(struct since_state *sn, struct data_file *df)
{
  struct stat st;
  int again;

  if(stat(df->d_name, &st) == 0){
    if((st.st_ino != df->d_ino) ||
       (st.st_dev != df->d_dev)){
      if(df->d_replaced == 0){
        df->d_replaced = 1;
        df->d_notable = 1;
      }
#ifdef DEBUG
      fprintf(stderr, "check: file %s no longer matches\n", df->d_name);
#endif
      /* TODO: could re-open filename here, redo lookup, etc */
      again = 1;
    } else {
      again = 0;
      /* TODO: could note a rename back to the old name */
      df->d_replaced = 0;
      df->d_moved = 0;
    }
  } else {
    switch(errno){
      case ENOENT :
        if(df->d_moved == 0){
          df->d_moved = 1;
          df->d_notable = 1;
        }
        break;
        /* TODO: could quit on some critical errors sooner */
    }
    again = 1;
  }

  if(again){
    if(fstat(df->d_fd, &st)){
      fprintf(stderr, "since: unable to stat %s: %s\n", df->d_name, strerror(errno));
      return -1;
    }
    if(st.st_nlink == 0){
      /* TODO: could delete entry to reduce inode collisions */
      if(df->d_deleted == 0){
        df->d_deleted = 1;
        df->d_notable = 1;
      }
    } else {
      if(df->d_moved == 0){
        df->d_moved = 1;
        df->d_notable = 1;
      }
    }
  }

#ifdef DEBUG
  fprintf(stderr, "update: new size=%Lu, old max=%Lu\n", st.st_size, df->d_now);
#endif

  if(st.st_size < df->d_now){
    fprintf(stderr, "since: considering %s to be truncated, displaying from start\n", df->d_name);
    df->d_had = 0;
    df->d_pos = 0;
    df->d_jump = 1;
    df->d_write = 1;
    df->d_notable = 1;
  }

  if(df->d_now < st.st_size){
    df->d_notable = 1;
  }
  df->d_now = st.st_size;

  return 0;
}