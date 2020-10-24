static int load_state_file(struct since_state *sn)
{
  struct stat st;
  int prot, flags;
  int rr;
  /* we assume that state file can fit into address space */
  unsigned int rt;

#ifdef DEBUG
  if(sn->s_buffer){
    fprintf(stderr, "since: logic problem: loading over unallocated buffer\n");
    abort();
  }
#endif

  sn->s_ismap = 0;

  if(sn->s_fd < 0){
    /* where there is no state file and invoked as readonly */
    return 1;
  }

  if(fstat(sn->s_fd, &st)){
    fprintf(stderr, "since: unable to stat %s: %s\n", sn->s_name, strerror(errno));
    return -1;
  }

  sn->s_size = st.st_size;
  if(sn->s_size == 0){
    return 0; /* empty, possibly new file */
  }

  if(sn->s_domap){
    /* not really clear if this buys us anything, actually may make updates non-atomic as we rely on mmap to write back changes */
    prot = PROT_READ;
    if(!(sn->s_readonly)) prot |= PROT_WRITE;
    flags = sn->s_atomic ? MAP_PRIVATE : MAP_SHARED;
    sn->s_buffer = mmap(NULL, sn->s_size, prot, flags, sn->s_fd, 0);
    if((void *)(sn->s_buffer) != MAP_FAILED){
#ifdef DEBUG
      fprintf(stderr, "since: have mapped %s (size=%u)\n", sn->s_name, sn->s_size);
#endif
      sn->s_ismap = 1;
      return 0;
    }
  }

  sn->s_buffer = malloc(sn->s_size);
  if(sn->s_buffer == NULL){
    fprintf(stderr, "since: unable to allocate %d bytes to load state file %s: %s\n", sn->s_size, sn->s_name, strerror(errno));
    return -1;
  }

  for(rt = 0; rt < sn->s_size;){
    rr = read(sn->s_fd, sn->s_buffer + rt, sn->s_size - rt);
    if(rr < 0){
      switch(errno){
        case EAGAIN :
        case EINTR :
          break;
        default :
          fprintf(stderr, "since: read of %s failed after %d bytes: %s\n", sn->s_name, rt, strerror(errno));
          return -1;
      }
    }
    if(rr == 0){
      fprintf(stderr, "since: premature end of %s: %d bytes outstanding\n", sn->s_name, (unsigned int)(sn->s_size) - rt);
      return -1;
    }
    rt += rr;
  }

#ifdef DEBUG
  fprintf(stderr, "since: have read %s (size=%u)\n", sn->s_name, sn->s_size);
#endif

  return 0;
}