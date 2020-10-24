void f_init(hfsfile *file, hfsvol *vol, long cnid, const char *name)
{
  int i;

  file->vol   = vol;
  file->parid = 0;

  strcpy(file->name, name);

  file->cat.cdrType          = cdrFilRec;
  file->cat.cdrResrv2        = 0;

  file->cat.u.fil.filFlags   = 0;
  file->cat.u.fil.filTyp     = 0;

  file->cat.u.fil.filUsrWds.fdType       = 0;
  file->cat.u.fil.filUsrWds.fdCreator    = 0;
  file->cat.u.fil.filUsrWds.fdFlags      = 0;
  file->cat.u.fil.filUsrWds.fdLocation.v = 0;
  file->cat.u.fil.filUsrWds.fdLocation.h = 0;
  file->cat.u.fil.filUsrWds.fdFldr       = 0;

  file->cat.u.fil.filFlNum   = cnid;
  file->cat.u.fil.filStBlk   = 0;
  file->cat.u.fil.filLgLen   = 0;
  file->cat.u.fil.filPyLen   = 0;
  file->cat.u.fil.filRStBlk  = 0;
  file->cat.u.fil.filRLgLen  = 0;
  file->cat.u.fil.filRPyLen  = 0;
  file->cat.u.fil.filCrDat   = 0;
  file->cat.u.fil.filMdDat   = 0;
  file->cat.u.fil.filBkDat   = 0;

  file->cat.u.fil.filFndrInfo.fdIconID = 0;
  for (i = 0; i < 4; ++i)
    file->cat.u.fil.filFndrInfo.fdUnused[i] = 0;
  file->cat.u.fil.filFndrInfo.fdComment = 0;
  file->cat.u.fil.filFndrInfo.fdPutAway = 0;

  file->cat.u.fil.filClpSize = 0;

  for (i = 0; i < 3; ++i)
    {
      file->cat.u.fil.filExtRec[i].xdrStABN     = 0;
      file->cat.u.fil.filExtRec[i].xdrNumABlks  = 0;

      file->cat.u.fil.filRExtRec[i].xdrStABN    = 0;
      file->cat.u.fil.filRExtRec[i].xdrNumABlks = 0;
    }

  file->cat.u.fil.filResrv   = 0;

  f_selectfork(file, fkData);

  file->flags = 0;

  file->prev  = 0;
  file->next  = 0;
}
