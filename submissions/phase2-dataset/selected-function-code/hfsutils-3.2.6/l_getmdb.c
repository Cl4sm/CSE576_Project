int l_getmdb(hfsvol *vol, MDB *mdb, int backup)
{
  block b;
  const byte *ptr = b;
  int i;

  if (b_readlb(vol, backup ? vol->vlen - 2 : 2, &b) == -1)
    goto fail;

  d_fetchsw(&ptr, &mdb->drSigWord);
  d_fetchsl(&ptr, &mdb->drCrDate);
  d_fetchsl(&ptr, &mdb->drLsMod);
  d_fetchsw(&ptr, &mdb->drAtrb);
  d_fetchuw(&ptr, &mdb->drNmFls);
  d_fetchuw(&ptr, &mdb->drVBMSt);
  d_fetchuw(&ptr, &mdb->drAllocPtr);
  d_fetchuw(&ptr, &mdb->drNmAlBlks);
  d_fetchul(&ptr, &mdb->drAlBlkSiz);
  d_fetchul(&ptr, &mdb->drClpSiz);
  d_fetchuw(&ptr, &mdb->drAlBlSt);
  d_fetchsl(&ptr, &mdb->drNxtCNID);
  d_fetchuw(&ptr, &mdb->drFreeBks);

  d_fetchstr(&ptr, mdb->drVN, sizeof(mdb->drVN));

  ASSERT(ptr - b == 64);

  d_fetchsl(&ptr, &mdb->drVolBkUp);
  d_fetchsw(&ptr, &mdb->drVSeqNum);
  d_fetchul(&ptr, &mdb->drWrCnt);
  d_fetchul(&ptr, &mdb->drXTClpSiz);
  d_fetchul(&ptr, &mdb->drCTClpSiz);
  d_fetchuw(&ptr, &mdb->drNmRtDirs);
  d_fetchul(&ptr, &mdb->drFilCnt);
  d_fetchul(&ptr, &mdb->drDirCnt);

  for (i = 0; i < 8; ++i)
    d_fetchsl(&ptr, &mdb->drFndrInfo[i]);

  ASSERT(ptr - b == 124);

  d_fetchuw(&ptr, &mdb->drEmbedSigWord);
  d_fetchuw(&ptr, &mdb->drEmbedExtent.xdrStABN);
  d_fetchuw(&ptr, &mdb->drEmbedExtent.xdrNumABlks);

  d_fetchul(&ptr, &mdb->drXTFlSize);

  for (i = 0; i < 3; ++i)
    {
      d_fetchuw(&ptr, &mdb->drXTExtRec[i].xdrStABN);
      d_fetchuw(&ptr, &mdb->drXTExtRec[i].xdrNumABlks);
    }

  ASSERT(ptr - b == 146);

  d_fetchul(&ptr, &mdb->drCTFlSize);

  for (i = 0; i < 3; ++i)
    {
      d_fetchuw(&ptr, &mdb->drCTExtRec[i].xdrStABN);
      d_fetchuw(&ptr, &mdb->drCTExtRec[i].xdrNumABlks);
    }

  ASSERT(ptr - b == 162);

  return 0;

fail:
  return -1;
}
