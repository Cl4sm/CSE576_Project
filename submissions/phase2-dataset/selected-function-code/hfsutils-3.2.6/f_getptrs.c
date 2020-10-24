void f_getptrs(hfsfile *file, ExtDataRec **extrec,
	       unsigned long **lglen, unsigned long **pylen)
{
  if (file->fork == fkData)
    {
      if (extrec)
	*extrec = &file->cat.u.fil.filExtRec;
      if (lglen)
	*lglen  = &file->cat.u.fil.filLgLen;
      if (pylen)
	*pylen  = &file->cat.u.fil.filPyLen;
    }
  else
    {
      if (extrec)
	*extrec = &file->cat.u.fil.filRExtRec;
      if (lglen)
	*lglen  = &file->cat.u.fil.filRLgLen;
      if (pylen)
	*pylen  = &file->cat.u.fil.filRPyLen;
    }
}
