void update_stats() {
  FILE *f;
  char line[1024];
  char hdname[200];
  int readok = 0;
  const char *proc_fname;
  if (!use_proc_diskstats) proc_fname = "/proc/partitions";
  else proc_fname = "/proc/diskstats";

  f = fopen(proc_fname, "r");
  if (!f) { perror(proc_fname); return; }
  while (fgets(line, sizeof(line), f)) {
    int major, minor;
    unsigned long nr, nw;
    const char *fmt = use_proc_diskstats ? 
      "%d %d %200s %*d %*d %lu %*d %*d %*d %lu" :
      "%d %d %*u %200s %*d %*d %lu %*d %*d %*d %lu";
    if (sscanf(line, fmt, &major, &minor, hdname, &nr, &nw) == 5 ||
        (use_proc_diskstats && is_partition(major,minor) &&
         /* .. kernel 2.5 uses a different format for partitions and disc */
         sscanf(line, "%d %d %200s %*d %lu %*d %lu", &major, &minor, hdname, &nr, &nw) == 5)) {
      DiskList *dl;
      if (readok == 0) readok = 1;
      if ((dl=find_dev(major,minor))) {
        dl->touched_r = (dl->nr - nr) ? 10 : dl->touched_r; 
        dl->touched_w = (dl->nw - nw) ? 10 : dl->touched_w; 
        dl->nr = nr; dl->nw = nw;
        if (is_displayed(dl->hd_id,dl->part_id) && 
            ((dl->part_id && (!find_id(dl->hd_id, 0) || !is_displayed(dl->hd_id, 0))) || /* partition without host disk */
             (dl->part_id == 0))) /* disk */
          {
            if (!Prefs.debug_disk_rd) {
              pstat_add(&ps.disk_read, nr);
            } else {
              static int cntr = 0; cntr+=(rand()%30) == 0 ? Prefs.debug_disk_rd : 0;
              pstat_add(&ps.disk_read, nr + cntr);
            }
            if (!Prefs.debug_disk_wr) {
              pstat_add(&ps.disk_write, nw);
            } else {
              static int cntw = 0; cntw+=(rand()%30) == 0 ? Prefs.debug_disk_wr : 0;
              pstat_add(&ps.disk_write, nw + cntw);
            }
            readok = 2;
          }
      }
      /*      if (in_our_disk || find_dev(major,minor))*/ {
        strlist *sl;
        for (sl = swap_list(); sl; sl=sl->next) {
          if (strcmp(stripdev(hdname), stripdev(sl->s)) == 0) {
            if (!Prefs.debug_swapio) {
              pstat_add(&ps.swap_in, nr);
              pstat_add(&ps.swap_out, nw);
            } else {
              static int cnt = 0; cnt+=Prefs.debug_swapio;
              pstat_add(&ps.swap_in, nr + cnt);
              pstat_add(&ps.swap_out, nw + cnt);
            }
          }
        }
      }
    }
  }
  fclose(f);
  pstat_advance(&ps.disk_read);pstat_advance(&ps.disk_write);
  pstat_advance(&ps.swap_in);pstat_advance(&ps.swap_out);
  if (readok == 0) { fprintf(stderr, "warning: could not find any info in %s (kernel too old?)\n", proc_fname); exit(1); }
  else if (readok == 1) { ONLY_ONCE(fprintf(stderr, "warning: could not find any monitored disc in %s\n", proc_fname)); }
  BLAHBLAH(1,printf("swap: %5.2f,%5.2f disc: %5.2f,%5.2f MB/s\n", 
                    get_swapin_throughput(), get_swapout_throughput(), 
                    get_read_throughput(), get_write_throughput()));
}