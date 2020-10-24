int showvol(mountent *ment)
{
  hfsvol *vol;
  hfsvolent vent;
  int result = 0;

  printf("Current volume is mounted from");
  if (ment->partno > 0)
    printf(" partition %d of", ment->partno);
  printf(":\n  %s\n", ment->path);

  vol = hfsutil_remount(ment, HFS_MODE_ANY);
  if (vol == 0)
    return 1;

  printf("\n");

  hfs_vstat(vol, &vent);
  hfsutil_pinfo(&vent);
  hfsutil_unmount(vol, &result);

  return result;
}
