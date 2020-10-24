hfsvol *hfsutil_remount(mountent *ment, int flags)
{
  hfsvol *vol;
  hfsvolent vent;

  if (ment == 0)
    {
      fprintf(stderr, "%s: No volume is current; use `hmount' or `hvol'\n",
	      argv0);
      return 0;
    }

  suid_enable();
  vol = hfs_mount(ment->path, ment->partno, flags);
  suid_disable();

  if (vol == 0)
    {
      hfsutil_perror(ment->path);
      return 0;
    }

  hfs_vstat(vol, &vent);

  if (strcmp(vent.name, ment->vname) != 0)
    {
      fprintf(stderr, "%s: Expected volume \"%s\" not found\n",
	      argv0, ment->vname);
      fprintf(stderr, "%s: Replace media on %s or use `hmount'\n",
	      argv0, ment->path);

      hfs_umount(vol);
      return 0;
    }

  if (hfs_chdir(vol, ment->cwd) == -1)
    {
      fprintf(stderr, "%s: Current HFS directory \"%s%s:\" no longer exists\n",
	      argv0, ment->vname, ment->cwd);
    }

  return vol;
}
