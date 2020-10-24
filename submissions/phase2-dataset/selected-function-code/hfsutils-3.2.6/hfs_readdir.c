{
  CatKeyRec key;
  CatDataRec data;
  const byte *ptr;

  if (dir->dirid == 0)
    {
      hfsvol *vol;
      char cname[HFS_MAX_FLEN + 1];

      for (vol = hfs_mounts; vol; vol = vol->next)
	{
	  if (vol == dir->vptr)
	    break;
	}

      if (vol == 0)
	ERROR(ENOENT, "no more entries");

      if (v_getdthread(vol, HFS_CNID_ROOTDIR, &data, 0) <= 0 ||
	  v_catsearch(vol, HFS_CNID_ROOTPAR, data.u.dthd.thdCName,
		      &data, cname, 0) <= 0)
	goto fail;

      r_unpackdirent(HFS_CNID_ROOTPAR, cname, &data, ent);

      dir->vptr = vol->next;

      goto done;
    }

  if (dir->n.rnum == -1)
    ERROR(ENOENT, "no more entries");

  while (1)
    {
      ++dir->n.rnum;

      while (dir->n.rnum >= dir->n.nd.ndNRecs)
	{
	  if (dir->n.nd.ndFLink == 0)
	    {
	      dir->n.rnum = -1;
	      ERROR(ENOENT, "no more entries");
	    }

	  if (bt_getnode(&dir->n, dir->n.bt, dir->n.nd.ndFLink) == -1)
	    {
	      dir->n.rnum = -1;
	      goto fail;
	    }

	  dir->n.rnum = 0;
	}

      ptr = HFS_NODEREC(dir->n, dir->n.rnum);

      r_unpackcatkey(ptr, &key);

      if (key.ckrParID != dir->dirid)
	{
	  dir->n.rnum = -1;
	  ERROR(ENOENT, "no more entries");
	}

      r_unpackcatdata(HFS_RECDATA(ptr), &data);

      switch (data.cdrType)
	{
	case cdrDirRec:
	case cdrFilRec:
	  r_unpackdirent(key.ckrParID, key.ckrCName, &data, ent);
	  goto done;

	case cdrThdRec:
	case cdrFThdRec:
	  break;

	default:
	  dir->n.rnum = -1;
	  ERROR(EIO, "unexpected directory entry found");
	}
    }

done:
  return 0;

fail:
  return -1;
}
