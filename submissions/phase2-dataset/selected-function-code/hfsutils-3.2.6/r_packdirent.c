{
  switch (data->cdrType)
    {
    case cdrDirRec:
      data->u.dir.dirCrDat = d_mtime(ent->crdate);
      data->u.dir.dirMdDat = d_mtime(ent->mddate);
      data->u.dir.dirBkDat = d_mtime(ent->bkdate);

      data->u.dir.dirUsrInfo.frFlags      = ent->fdflags;
      data->u.dir.dirUsrInfo.frLocation.v = ent->fdlocation.v;
      data->u.dir.dirUsrInfo.frLocation.h = ent->fdlocation.h;

      data->u.dir.dirUsrInfo.frRect.top    = ent->u.dir.rect.top;
      data->u.dir.dirUsrInfo.frRect.left   = ent->u.dir.rect.left;
      data->u.dir.dirUsrInfo.frRect.bottom = ent->u.dir.rect.bottom;
      data->u.dir.dirUsrInfo.frRect.right  = ent->u.dir.rect.right;

      break;

    case cdrFilRec:
      if (ent->flags & HFS_ISLOCKED)
	data->u.fil.filFlags |=  (1 << 0);
      else
	data->u.fil.filFlags &= ~(1 << 0);

      data->u.fil.filCrDat = d_mtime(ent->crdate);
      data->u.fil.filMdDat = d_mtime(ent->mddate);
      data->u.fil.filBkDat = d_mtime(ent->bkdate);

      data->u.fil.filUsrWds.fdFlags      = ent->fdflags;
      data->u.fil.filUsrWds.fdLocation.v = ent->fdlocation.v;
      data->u.fil.filUsrWds.fdLocation.h = ent->fdlocation.h;

      data->u.fil.filUsrWds.fdType =
	d_getsl((const unsigned char *) ent->u.file.type);
      data->u.fil.filUsrWds.fdCreator =
	d_getsl((const unsigned char *) ent->u.file.creator);

      break;
    }
}
