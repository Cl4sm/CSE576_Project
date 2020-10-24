static
char *direntstr(hfsdirent *ent)
{
  char
    cnid[CHARLEN(unsigned long) + 1],
    parid[CHARLEN(unsigned long) + 1],
    rsize[CHARLEN(unsigned long) + 1],
    dsize[CHARLEN(unsigned long) + 1],
    crdate[CHARLEN(long) + 1],
    mddate[CHARLEN(long) + 1],
    bkdate[CHARLEN(long) + 1];
  register int argc;
  char *argv[24];
  int locked, invis;

  argc = 0;

  argv[argc++] = "name";
  argv[argc++] = ent->name;

  sprintf(cnid,   "%lu", ent->cnid);
  sprintf(parid,  "%lu", ent->parid);

  argv[argc++] = "cnid";
  argv[argc++] = cnid;

  argv[argc++] = "parid";
  argv[argc++] = parid;

  argv[argc++] = "kind";

  if (ent->flags & HFS_ISDIR)
    {
      argv[argc++] = "directory";

      sprintf(dsize, "%u", ent->u.dir.valence);

      argv[argc++] = "size";
      argv[argc++] = dsize;

      argv[argc++] = "flags";
      argv[argc++] = (ent->fdflags & HFS_FNDR_ISINVISIBLE) ? "invis" : "";
    }
  else
    {
      argv[argc++] = "file";

      argv[argc++] = "type";
      argv[argc++] = ent->u.file.type;

      argv[argc++] = "creator";
      argv[argc++] = ent->u.file.creator;

      locked = ent->flags & HFS_ISLOCKED;
      invis  = ent->fdflags & HFS_FNDR_ISINVISIBLE;

      argv[argc++] = "flags";
      argv[argc++] = (locked && invis) ? "locked invis" :
	(locked ? "locked" : (invis ? "invis" : ""));

      sprintf(rsize,  "%lu", ent->u.file.rsize);
      sprintf(dsize,  "%lu", ent->u.file.dsize);

      argv[argc++] = "rsize";
      argv[argc++] = rsize;

      argv[argc++] = "dsize";
      argv[argc++] = dsize;
    }

  sprintf(crdate, "%ld", (long) ent->crdate);
  sprintf(mddate, "%ld", (long) ent->mddate);
  sprintf(bkdate, "%ld", (long) ent->bkdate);

  argv[argc++] = "crdate";
  argv[argc++] = crdate;

  argv[argc++] = "mddate";
  argv[argc++] = mddate;

  argv[argc++] = "bkdate";
  argv[argc++] = bkdate;

  return Tcl_Merge(argc, argv);
}
