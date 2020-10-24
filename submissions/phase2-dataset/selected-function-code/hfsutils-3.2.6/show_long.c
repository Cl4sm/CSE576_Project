void show_long(int sz, queueent *ents, char **strs,
	       int flags, int options, int width)
{
  int i;
  time_t now;

  now = time(0);

  for (i = 0; i < sz; ++i)
    {
      hfsdirent *ent;
      time_t when;
      char timebuf[26];

      ent = &ents[i].dirent;

      switch (options & T_MASK)
	{
	case T_MOD:
	  when = ent->mddate;
	  break;

	case T_CREATE:
	  when = ent->crdate;
	  break;

	default:
	  abort();
	}

      strcpy(timebuf, ctime(&when));

      if (now > when + 6L * 30L * 24L * 60L * 60L ||
	  now < when - 60L * 60L)
	strcpy(timebuf + 11, timebuf + 19);

      timebuf[16] = 0;

      showmisc(ent, flags);

      if (ent->flags & HFS_ISDIR)
	printf("d%c %9u item%c               %s %s\n",
	       ent->fdflags & HFS_FNDR_ISINVISIBLE ? 'i' : ' ',
	       ent->u.dir.valence, ent->u.dir.valence == 1 ? ' ' : 's',
	       timebuf + 4, strs[i]);
      else
	printf("%c%c %4s/%4s %9lu %9lu %s %s\n",
	       ent->flags & HFS_ISLOCKED ? 'F' : 'f',
	       ent->fdflags & HFS_FNDR_ISINVISIBLE ? 'i' : ' ',
	       ent->u.file.type, ent->u.file.creator,
	       ent->u.file.rsize, ent->u.file.dsize,
	       timebuf + 4, strs[i]);
    }
}
