static int internal_update_state_file(struct since_state *sn)
{
  int sofar, result;

#if 0 /* not needed, always run outof tmp where a new file is opened */
  if(lseek(sn->s_fd, 0, SEEK_SET) != 0){
    fprintf(stderr, "since: unable to seek to start of file %s: %s\n", sn->s_name, strerror(errno));
    return -1;
  }
#endif

  sofar = 0;
  while(sofar < sn->s_size){
    result = write(sn->s_fd, sn->s_buffer + sofar, sn->s_size - sofar);
    if(result < 0){
      switch(errno){
        case EAGAIN :
        case EINTR  :
          break;
        default :
          fprintf(stderr, "since: unable to rewrite %d bytes to %s: %s\n", sn->s_size - sofar, sn->s_name, strerror(errno));
          return -1;
      }
    } else {
      sofar += result;
      if(result == 0){
        fprintf(stderr, "since: warning: wrote 0 bytes to file to %s\n", sn->s_name);
      }
    }
  }

  return append_state_file(sn);
}