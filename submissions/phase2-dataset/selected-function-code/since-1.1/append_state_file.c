static int append_state_file(struct since_state *sn)
{
  int sofar, result;

  if((sn->s_append == NULL) || (sn->s_add == 0)){
    return 0;
  }

  if(lseek(sn->s_fd, sn->s_size, SEEK_SET) != sn->s_size){
    fprintf(stderr, "since: unable to seek to end of file %s: %s\n", sn->s_name, strerror(errno));
    return -1;
  }

  sofar = 0;
  while(sofar < sn->s_add){
    result = write(sn->s_fd, sn->s_append + sofar, sn->s_add - sofar);
    if(result < 0){
      switch(errno){
        case EAGAIN :
        case EINTR  :
          break;
        default :
          fprintf(stderr, "since: unable to write %d bytes to %s: %s\n", sn->s_add - sofar, sn->s_name, strerror(errno));
          return -1;
      }
    } else {
      sofar += result;
      if(result == 0){
        fprintf(stderr, "since: warning: wrote 0 bytes to file to %s\n", sn->s_name);
      }
    }
  }

  return 0;
}