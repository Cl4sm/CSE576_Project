void r_unpackcatdata(const byte *pdata, CatDataRec *data)
{
  int i;

  d_fetchsb(&pdata, &data->cdrType);
  d_fetchsb(&pdata, &data->cdrResrv2);

  switch (data->cdrType)
    {
    case cdrDirRec:
      d_fetchsw(&pdata, &data->u.dir.dirFlags);
      d_fetchuw(&pdata, &data->u.dir.dirVal);
      d_fetchul(&pdata, &data->u.dir.dirDirID);
      d_fetchsl(&pdata, &data->u.dir.dirCrDat);
      d_fetchsl(&pdata, &data->u.dir.dirMdDat);
      d_fetchsl(&pdata, &data->u.dir.dirBkDat);

      d_fetchsw(&pdata, &data->u.dir.dirUsrInfo.frRect.top);
      d_fetchsw(&pdata, &data->u.dir.dirUsrInfo.frRect.left);
      d_fetchsw(&pdata, &data->u.dir.dirUsrInfo.frRect.bottom);
      d_fetchsw(&pdata, &data->u.dir.dirUsrInfo.frRect.right);
      d_fetchsw(&pdata, &data->u.dir.dirUsrInfo.frFlags);
      d_fetchsw(&pdata, &data->u.dir.dirUsrInfo.frLocation.v);
      d_fetchsw(&pdata, &data->u.dir.dirUsrInfo.frLocation.h);
      d_fetchsw(&pdata, &data->u.dir.dirUsrInfo.frView);

      d_fetchsw(&pdata, &data->u.dir.dirFndrInfo.frScroll.v);
      d_fetchsw(&pdata, &data->u.dir.dirFndrInfo.frScroll.h);
      d_fetchsl(&pdata, &data->u.dir.dirFndrInfo.frOpenChain);
      d_fetchsw(&pdata, &data->u.dir.dirFndrInfo.frUnused);
      d_fetchsw(&pdata, &data->u.dir.dirFndrInfo.frComment);
      d_fetchsl(&pdata, &data->u.dir.dirFndrInfo.frPutAway);

      for (i = 0; i < 4; ++i)
	d_fetchsl(&pdata, &data->u.dir.dirResrv[i]);

      break;

    case cdrFilRec:
      d_fetchsb(&pdata, &data->u.fil.filFlags);
      d_fetchsb(&pdata, &data->u.fil.filTyp);

      d_fetchsl(&pdata, &data->u.fil.filUsrWds.fdType);
      d_fetchsl(&pdata, &data->u.fil.filUsrWds.fdCreator);
      d_fetchsw(&pdata, &data->u.fil.filUsrWds.fdFlags);
      d_fetchsw(&pdata, &data->u.fil.filUsrWds.fdLocation.v);
      d_fetchsw(&pdata, &data->u.fil.filUsrWds.fdLocation.h);
      d_fetchsw(&pdata, &data->u.fil.filUsrWds.fdFldr);

      d_fetchul(&pdata, &data->u.fil.filFlNum);

      d_fetchuw(&pdata, &data->u.fil.filStBlk);
      d_fetchul(&pdata, &data->u.fil.filLgLen);
      d_fetchul(&pdata, &data->u.fil.filPyLen);

      d_fetchuw(&pdata, &data->u.fil.filRStBlk);
      d_fetchul(&pdata, &data->u.fil.filRLgLen);
      d_fetchul(&pdata, &data->u.fil.filRPyLen);

      d_fetchsl(&pdata, &data->u.fil.filCrDat);
      d_fetchsl(&pdata, &data->u.fil.filMdDat);
      d_fetchsl(&pdata, &data->u.fil.filBkDat);

      d_fetchsw(&pdata, &data->u.fil.filFndrInfo.fdIconID);
      for (i = 0; i < 4; ++i)
	d_fetchsw(&pdata, &data->u.fil.filFndrInfo.fdUnused[i]);
      d_fetchsw(&pdata, &data->u.fil.filFndrInfo.fdComment);
      d_fetchsl(&pdata, &data->u.fil.filFndrInfo.fdPutAway);

      d_fetchuw(&pdata, &data->u.fil.filClpSize);

      for (i = 0; i < 3; ++i)
	{
	  d_fetchuw(&pdata, &data->u.fil.filExtRec[i].xdrStABN);
	  d_fetchuw(&pdata, &data->u.fil.filExtRec[i].xdrNumABlks);
	}

      for (i = 0; i < 3; ++i)
	{
	  d_fetchuw(&pdata, &data->u.fil.filRExtRec[i].xdrStABN);
	  d_fetchuw(&pdata, &data->u.fil.filRExtRec[i].xdrNumABlks);
	}

      d_fetchsl(&pdata, &data->u.fil.filResrv);

      break;

    case cdrThdRec:
      for (i = 0; i < 2; ++i)
	d_fetchsl(&pdata, &data->u.dthd.thdResrv[i]);

      d_fetchul(&pdata, &data->u.dthd.thdParID);

      d_fetchstr(&pdata, data->u.dthd.thdCName,
		 sizeof(data->u.dthd.thdCName));

      break;

    case cdrFThdRec:
      for (i = 0; i < 2; ++i)
	d_fetchsl(&pdata, &data->u.fthd.fthdResrv[i]);

      d_fetchul(&pdata, &data->u.fthd.fthdParID);

      d_fetchstr(&pdata, data->u.fthd.fthdCName,
		 sizeof(data->u.fthd.fthdCName));

      break;

    default:
      ASSERT(0);
    }
}
