void scan_all_hd(int add_partitions) {
  char s[512];
  FILE *f = NULL;
  /* find mounted partitions */
  if (add_partitions && (f = fopen("/etc/mtab","r"))) {
    while (fgets(s,512,f)) {
      char partname[512], mountpoint[512];
      mountpoint[0] = 0;
      if (sscanf(s,"%500s %500s", partname, mountpoint)>=1) {
        char *p = strchr(mountpoint, '/');
        //printf("add_device_by_name(%s, %s)\n", partname, p);
        add_device_by_name(partname, p);
      }
    }
  }
  if (f) fclose(f);

  /* second try .. look for hard drives listed in proc/partitions (and only hard-drives .. their partitions
   are supposed to be listed in mtab) */
  if ((f = fopen("/proc/partitions","r"))) {
    while (fgets(s,512,f)) {
      unsigned major, minor;
      char name[512]; name[0] = 0;
      if (sscanf(s,"%d %d %*d %500s", &major, &minor, name)==3) {        
        int hd_id, part_id;
        /* on n'ajoute que les disques dont au moins une partoche etait dans mtab */
        if (device_info(major,minor,NULL,&hd_id,&part_id) != 0 && part_id == 0 && find_id(hd_id,-1)) {
          add_device_by_id(major,minor,NULL);
        }
      }
    }
  }

  if (f) fclose(f);
  /*  for (c = 'a'; c <= 'z'; ++c) {
    snprintf(s,512,"/proc/ide/hd%c/media",c);
    if ((f = fopen(s,"r")) && fgets(s, 512, f) && strcmp(s, "disk\n")==0) {
      snprintf(s, 512, "/dev/hd%c",c);
      add_device(s, NULL);
    }
    if (f) fclose(f);
    }*/
}