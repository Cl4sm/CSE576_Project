void r_unpackdirent(unsigned long parid, const char *name,
		    const CatDataRec *data, hfsdirent *ent)
{
  strcpy(ent->name, name);
  ent->parid = parid;

  switch (data->cdrType)
    {
    case cdrDirRec:
      ent->flags = HFS_ISDIR;
      ent->cnid  = data->u.dir.dirDirID;

      ent->crdate = d_ltime(data->u.dir.dirCrDat);
      ent->mddate = d_ltime(data->u.dir.dirMdDat);
      ent->bkdate = d_ltime(data->u.dir.dirBkDat);

      ent->fdflags      = data->u.dir.dirUsrInfo.frFlags;
      ent->fdlocation.v = data->u.dir.dirUsrInfo.frLocation.v;
      ent->fdlocation.h = data->u.dir.dirUsrInfo.frLocation.h;

      ent->u.dir.valence = data->u.dir.dirVal;

      ent->u.dir.rect.top    = data->u.dir.dirUsrInfo.frRect.top;
      ent->u.dir.rect.left   = data->u.dir.dirUsrInfo.frRect.left;
      ent->u.dir.rect.bottom = data->u.dir.dirUsrInfo.frRect.bottom;
      ent->u.dir.rect.right  = data->u.dir.dirUsrInfo.frRect.right;

      break;

    case cdrFilRec:
      ent->flags = (data->u.fil.filFlags & (1 << 0)) ? HFS_ISLOCKED : 0;
      ent->cnid  = data->u.fil.filFlNum;

      ent->crdate = d_ltime(data->u.fil.filCrDat);
      ent->mddate = d_ltime(data->u.fil.filMdDat);
      ent->bkdate = d_ltime(data->u.fil.filBkDat);

      ent->fdflags      = data->u.fil.filUsrWds.fdFlags;
      ent->fdlocation.v = data->u.fil.filUsrWds.fdLocation.v;
      ent->fdlocation.h = data->u.fil.filUsrWds.fdLocation.h;

      ent->u.file.dsize = data->u.fil.filLgLen;
      ent->u.file.rsize = data->u.fil.filRLgLen;

      d_putsl((unsigned char *) ent->u.file.type,
	      data->u.fil.filUsrWds.fdType);
      d_putsl((unsigned char *) ent->u.file.creator,
	     data->u.fil.filUsrWds.fdCreator);

      ent->u.file.type[4] = ent->u.file.creator[4] = 0;

      break;
    }
}
