static int display_file(struct since_state *sn, struct data_file *df, int single)
{
  char *ptr;
  char buffer[IO_BUFFER];
  int rr, result;
  off_t range, i;
  unsigned int fixup;

  /* WARNING: should not manipulate d_had here, should be done in lookup and refresh, maybe pos resets too */
  if(df->d_had > df->d_now){
    fprintf(stderr, "since: considering %s to be truncated, displaying from start\n", df->d_name);
    df->d_had = 0;
    /* WARNING: d_pos gets saved, not d_had */
    df->d_write = 1;
  }

  if(df->d_pos < df->d_had){
    df->d_jump = 1;
    df->d_pos = df->d_had;
  }

#ifdef DEBUG
  if(df->d_pos > df->d_now){
    fprintf(stderr, "since: logic failure - position pointer has overtaken length\n");
    abort();
  }
#endif

  range = df->d_now - df->d_pos;
  if(range == 0){
    display_header(sn, df, single, 0);
    return 0;
  }

  if((range > IO_BUFFER) && sn->s_domap){
    fixup = df->d_pos & (IO_BUFFER - 1);
    ptr = mmap(NULL, range + range, PROT_READ, MAP_PRIVATE, df->d_fd, df->d_pos - fixup);
    if((void *)(ptr) != MAP_FAILED){
      display_header(sn, df, single, 0);

      result = display_buffer(sn, df, ptr + fixup, range);
      df->d_jump = 1;
      munmap(ptr, range + fixup);

      return result;
#ifdef DEBUG
    } else {
      fprintf(stderr, "display: mmap of %d failed: %s\n", df->d_fd, strerror(errno));
#endif
    }
  }

  if(df->d_jump){
    if(lseek(df->d_fd, df->d_pos, SEEK_SET) != df->d_pos){
      fprintf(stderr, "since: unable to seek in file %s: %s\n", df->d_name, strerror(errno));
      return -1;
    }
    df->d_jump = 0;
  }

  display_header(sn, df, single, 0);
  i = 0;
  df->d_write = 1;
  while(df->d_pos < df->d_now){
    rr = read(df->d_fd, buffer, IO_BUFFER);
    switch(rr){
      case -1 :
        switch(errno){
          case EAGAIN :
          case EINTR  :
            break;
          default :
            fprintf(stderr, "since: unable to read from %s: %s\n", df->d_name, strerror(errno));
            return -1;
        }
        break;
      case  0 :
        fprintf(stderr, "since: unexpected eof while reading from %s\n", df->d_name);
        break;
      default :
        result = display_buffer(sn, df, buffer, rr);
        if(result != 0){
          return result;
        }
        break;
    }
  }

  if(df->d_now < df->d_pos){
    /* in case more gets append to file while reading */
    df->d_now = df->d_pos;
  }

  return 0;
}