{
  const byte *start = pdata;
  int i;

  d_storesb(&pdata, data->cdrType);
  d_storesb(&pdata, data->cdrResrv2);

  switch (data->cdrType)
    {
    case cdrDirRec:
      d_storesw(&pdata, data->u.dir.dirFlags);
      d_storeuw(&pdata, data->u.dir.dirVal);
      d_storeul(&pdata, data->u.dir.dirDirID);
      d_storesl(&pdata, data->u.dir.dirCrDat);
      d_storesl(&pdata, data->u.dir.dirMdDat);
      d_storesl(&pdata, data->u.dir.dirBkDat);

      d_storesw(&pdata, data->u.dir.dirUsrInfo.frRect.top);
      d_storesw(&pdata, data->u.dir.dirUsrInfo.frRect.left);
      d_storesw(&pdata, data->u.dir.dirUsrInfo.frRect.bottom);
      d_storesw(&pdata, data->u.dir.dirUsrInfo.frRect.right);
      d_storesw(&pdata, data->u.dir.dirUsrInfo.frFlags);
      d_storesw(&pdata, data->u.dir.dirUsrInfo.frLocation.v);
      d_storesw(&pdata, data->u.dir.dirUsrInfo.frLocation.h);
      d_storesw(&pdata, data->u.dir.dirUsrInfo.frView);

      d_storesw(&pdata, data->u.dir.dirFndrInfo.frScroll.v);
      d_storesw(&pdata, data->u.dir.dirFndrInfo.frScroll.h);
      d_storesl(&pdata, data->u.dir.dirFndrInfo.frOpenChain);
      d_storesw(&pdata, data->u.dir.dirFndrInfo.frUnused);
      d_storesw(&pdata, data->u.dir.dirFndrInfo.frComment);
      d_storesl(&pdata, data->u.dir.dirFndrInfo.frPutAway);

      for (i = 0; i < 4; ++i)
	d_storesl(&pdata, data->u.dir.dirResrv[i]);

      break;

    case cdrFilRec:
      d_storesb(&pdata, data->u.fil.filFlags);
      d_storesb(&pdata, data->u.fil.filTyp);

      d_storesl(&pdata, data->u.fil.filUsrWds.fdType);
      d_storesl(&pdata, data->u.fil.filUsrWds.fdCreator);
      d_storesw(&pdata, data->u.fil.filUsrWds.fdFlags);
      d_storesw(&pdata, data->u.fil.filUsrWds.fdLocation.v);
      d_storesw(&pdata, data->u.fil.filUsrWds.fdLocation.h);
      d_storesw(&pdata, data->u.fil.filUsrWds.fdFldr);

      d_storeul(&pdata, data->u.fil.filFlNum);

      d_storeuw(&pdata, data->u.fil.filStBlk);
      d_storeul(&pdata, data->u.fil.filLgLen);
      d_storeul(&pdata, data->u.fil.filPyLen);

      d_storeuw(&pdata, data->u.fil.filRStBlk);
      d_storeul(&pdata, data->u.fil.filRLgLen);
      d_storeul(&pdata, data->u.fil.filRPyLen);

      d_storesl(&pdata, data->u.fil.filCrDat);
      d_storesl(&pdata, data->u.fil.filMdDat);
      d_storesl(&pdata, data->u.fil.filBkDat);

      d_storesw(&pdata, data->u.fil.filFndrInfo.fdIconID);
      for (i = 0; i < 4; ++i)
	d_storesw(&pdata, data->u.fil.filFndrInfo.fdUnused[i]);
      d_storesw(&pdata, data->u.fil.filFndrInfo.fdComment);
      d_storesl(&pdata, data->u.fil.filFndrInfo.fdPutAway);

      d_storeuw(&pdata, data->u.fil.filClpSize);

      for (i = 0; i < 3; ++i)
	{
	  d_storeuw(&pdata, data->u.fil.filExtRec[i].xdrStABN);
	  d_storeuw(&pdata, data->u.fil.filExtRec[i].xdrNumABlks);
	}

      for (i = 0; i < 3; ++i)
	{
	  d_storeuw(&pdata, data->u.fil.filRExtRec[i].xdrStABN);
	  d_storeuw(&pdata, data->u.fil.filRExtRec[i].xdrNumABlks);
	}

      d_storesl(&pdata, data->u.fil.filResrv);

      break;

    case cdrThdRec:
      for (i = 0; i < 2; ++i)
	d_storesl(&pdata, data->u.dthd.thdResrv[i]);

      d_storeul(&pdata, data->u.dthd.thdParID);

      d_storestr(&pdata, data->u.dthd.thdCName,
		 sizeof(data->u.dthd.thdCName));

      break;

    case cdrFThdRec:
      for (i = 0; i < 2; ++i)
	d_storesl(&pdata, data->u.fthd.fthdResrv[i]);

      d_storeul(&pdata, data->u.fthd.fthdParID);

      d_storestr(&pdata, data->u.fthd.fthdCName,
		 sizeof(data->u.fthd.fthdCName));

      break;

    default:
      ASSERT(0);
    }

  if (len)
    *len += pdata - start;
}
