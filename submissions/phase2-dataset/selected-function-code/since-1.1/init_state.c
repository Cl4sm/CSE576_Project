static void init_state(struct since_state *sn)
{
  struct stat st;

  sn->s_arch_device = sizeof(st.st_dev);
  sn->s_arch_inode = sizeof(st.st_ino);
  sn->s_arch_size = sizeof(st.st_size);

  sn->s_disk_device = sn->s_arch_device;
  sn->s_disk_inode = sn->s_arch_inode;
  sn->s_disk_size = sn->s_arch_size;

  sn->s_fmt_used = 0;
  sn->s_fmt_output = 0;
  sn->s_fmt_prefix = 0;

  sn->s_error = 0;
  sn->s_readonly = 0;
  sn->s_verbose = 1;
  sn->s_relaxed = 0;
  sn->s_follow = 0;
  sn->s_discard = 0;
  sn->s_delay.tv_sec = 1;
  sn->s_delay.tv_nsec = 0;
  sn->s_atomic = 0;
  sn->s_domap = 1;
  sn->s_nozip = 0;

  sn->s_name = NULL;
  sn->s_fd = (-1);

  sn->s_size = 0;
  sn->s_buffer = NULL;
  sn->s_ismap = 0;

  sn->s_add = 0;
  sn->s_append = NULL;

  sn->s_data_files = NULL;
  sn->s_data_count = 0;

  sn->s_notify = (-1);

  sn->s_header = stdout;
}