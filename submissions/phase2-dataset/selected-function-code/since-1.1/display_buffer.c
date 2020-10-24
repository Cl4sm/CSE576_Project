static int display_buffer(struct since_state *sn, struct data_file *df, char *buffer, unsigned int len)
{
  int wr, result;
  unsigned int wt, i, back;

#ifdef DEBUG
  sleep(1);

  fprintf(stderr, "display: need to display %u bytes for %s\n", len, df->d_name);
  if(len <= 0){
    fprintf(stderr, "since: logic failure: writing out empty string\n");
  }
#endif

  wt = 0;
  result = 1; /* used to infer signal */
  since_run = 1;
  sigprocmask(SIG_UNBLOCK, &(sn->s_set), NULL);

  while(since_run){
    wr = write(STDOUT_FILENO, buffer + wt, len - wt);
    switch(wr){
      case -1 :
#ifdef DEBUG
        fprintf(stderr, "display: write failed: %s\n", strerror(errno));
#endif
        switch(errno){
          case EINTR :
          case EPIPE :
            since_run = 0;
            result = 1;
          case EAGAIN : /* iffy */
            break;
          default :
            /* unlikely to do anything */
            fprintf(stderr, "since: unable to display output: %s\n", strerror(errno));
            since_run = 0;
            result = (-1);
        }
        break;
      case 0 :
        /* WARNING: unsure how to deal with this one, should we exit */
        break;
      default :
        wt += wr;
        if(wt >= len){
          since_run = 0;
          result = 0;
        }
        break;
    }
  }

  sigprocmask(SIG_BLOCK, &(sn->s_set), NULL);
  since_run = 1;

#ifdef DEBUG
  fprintf(stderr, "display: final code is %d, wt %u\n", result, wt);
#endif

  if(result < 0){ /* conventional error */
    return (-1);
  }

  if(result == 0){ /* success */
    df->d_pos += len;
    df->d_write = 1;
    return 0;
  }

  /* user interrupt */

  if(wt <= 0){
    return 1;
  }

  /* no guarantees, just trying to restart on a new line */
  back = (wt <= LINE_SEARCH) ? 0 : (wt - LINE_SEARCH);
  i = wt;

  do{
    i--;
    if(buffer[i] == '\n'){
#ifdef DEBUG
      fprintf(stderr, "recover: to newline at %u\n", i);
#endif
      wt = i + 1;
      break;
    }
  } while(i > back);

#ifdef DEBUG
  fprintf(stderr, "interrupt: at position %u\n", wt);
#endif

  df->d_pos += wt;
  df->d_write = 1;

  return 1;
}