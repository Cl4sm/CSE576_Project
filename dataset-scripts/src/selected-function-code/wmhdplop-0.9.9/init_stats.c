void init_stats(float update_interval) {
  char s[512];
  FILE *f;

  pstat_init(&ps.swap_in, (int)(0.5/update_interval)+1, update_interval);
  pstat_init(&ps.swap_out, (int)(0.5/update_interval)+1, update_interval);
  pstat_init(&ps.disk_read, (int)(0.5/update_interval)+1, update_interval);
  pstat_init(&ps.disk_write, (int)(0.5/update_interval)+1, update_interval);
  f = fopen("/proc/swaps","r");
  //if (!f) { perror("/proc/swaps"); exit(1); }
  if (f) {
    while (fgets(s, 512, f)) {
      char *s2 = strchr(s,' ');
      if (s2 && s2 != s && strncmp(s, "/dev/", 5) == 0) {
        *s2 = 0;
        add_swap(s);
        BLAHBLAH(1,printf("found swap partition: %s\n", swap_list()->s));
      }
    }
    fclose(f);
  }
  if (!swap_list()) {
    fprintf(stderr, "Warning: no swap partition found in /proc/swaps !!\n");
  }
  if ((f=fopen("/proc/diskstats","r"))) { 
    use_proc_diskstats = 1; fclose(f); 
  }
  else { use_proc_diskstats = 0; }
  BLAHBLAH(1,printf("using %s for disc statistics\n", use_proc_diskstats ? "/proc/diskstats" : "/proc/partitions"));
}