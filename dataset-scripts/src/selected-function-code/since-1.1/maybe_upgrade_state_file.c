static int maybe_upgrade_state_file(struct since_state *sn)
{
  if((sn->s_disk_device >= sn->s_arch_device) &&
     (sn->s_disk_inode >= sn->s_arch_inode) &&
     (sn->s_disk_size >= sn->s_arch_size)){
    if(sn->s_verbose > 4){
      fprintf(stderr, "since: state file data fields greater or equal to architecture, no rewrite needed\n");
    }
    return 0;
  }

  if(sn->s_readonly){
    fprintf(stderr, "since: need to rewrite state file %s, but invoked as readonly\n", sn->s_name);
    return -1;
  }

  if((sn->s_buffer == NULL) || (sn->s_size == 0)){
    fprintf(stderr, "since: major logic failure, attempting to upgrade empty file\n");
    return -1;
  }

  if(tmp_state_file(sn, &internal_upgrade_state_file)){
    return -1;
  }

  forget_state_file(sn);

  return load_state_file(sn);
}