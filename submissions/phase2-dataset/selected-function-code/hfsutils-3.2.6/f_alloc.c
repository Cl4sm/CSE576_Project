{
  hfsvol *vol = file->vol;
  unsigned long clumpsz;
  ExtDescriptor blocks;

  clumpsz = file->cat.u.fil.filClpSize;
  if (clumpsz == 0)
    {
      if (file == &vol->ext.f)
	clumpsz = vol->mdb.drXTClpSiz;
      else if (file == &vol->cat.f)
	clumpsz = vol->mdb.drCTClpSiz;
      else
	clumpsz = vol->mdb.drClpSiz;
    }

  blocks.xdrNumABlks = clumpsz / vol->mdb.drAlBlkSiz;

  if (v_allocblocks(vol, &blocks) == -1)
    goto fail;

  if (f_addextent(file, &blocks) == -1)
    {
      v_freeblocks(vol, &blocks);
      goto fail;
    }

  return blocks.xdrNumABlks;

fail:
  return -1;
}
