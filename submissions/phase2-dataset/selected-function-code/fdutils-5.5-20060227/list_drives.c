void list_drives() {
    mnt_node *mnt;
    int i,type;
    char devname[10];

    read_mtab();
/*
  printf("NAME  DEVICE     TYPE MOUNTPOINT STATUS\n");
  */
    printf("NAME   TYPE  STATUS\n");

    for(i=0;i<4;i++) {
	sprintf(devname,DEVICE_PATTERN,i);   
	type=probe_drive(devname);
	if (type) {
	    mnt=get_mounted(devname);
	    if (mnt) {
		printf(" " NAME_PATTERN "  %4dK  mounted on %s (%s %s)\n",
		       i,type,
		       mnt->ms.mnt_dir,
		       mnt->ms.mnt_type,
		       mnt->ms.mnt_opts);
	    }
	    else {
		printf(" " NAME_PATTERN "  %4dK  not mounted\n",
		       i,type);
	    }
	}
    }
}
