static InFile *CreateInFile(char *zArg, int *pnErr){
  int nSrc;
  char *zSrc;
  InFile *pFile;
  int i;

  /* 
  ** Get the name of the input file to be scanned
  */
  zSrc = zArg;
  for(nSrc=0; zSrc[nSrc] && zArg[nSrc]!=':'; nSrc++){}
  pFile = SafeMalloc( sizeof(InFile) );
  memset(pFile,0,sizeof(InFile));
  pFile->zSrc = StrDup(zSrc,nSrc);

  /* Figure out if we are dealing with C or C++ code.  Assume any
  ** file with ".c" or ".h" is C code and all else is C++.
  */
  if( nSrc>2 && zSrc[nSrc-2]=='.' && (zSrc[nSrc-1]=='c' || zSrc[nSrc-1]=='h')){
    pFile->flags &= ~DP_Cplusplus;
  }else{
    pFile->flags |= DP_Cplusplus;
  }

  /*
  ** If a separate header file is specified, use it
  */
  if( zSrc[nSrc]==':' ){
    int nHdr;
    char *zHdr;
    zHdr = &zSrc[nSrc+1];
    for(nHdr=0; zHdr[nHdr] && zHdr[nHdr]!=':'; nHdr++){}
    pFile->zHdr = StrDup(zHdr,nHdr);
  }

  /* Look for any 'c' or 'C' in the suffix of the file name and change
  ** that character to 'h' or 'H' respectively.  If no 'c' or 'C' is found,
  ** then assume we are dealing with a header.
  */
  else{
    int foundC = 0;
    pFile->zHdr = StrDup(zSrc,nSrc);
    for(i = nSrc-1; i>0 && pFile->zHdr[i]!='.'; i--){
      if( pFile->zHdr[i]=='c' ){
        foundC = 1;
        pFile->zHdr[i] = 'h';
      }else if( pFile->zHdr[i]=='C' ){
        foundC = 1;
        pFile->zHdr[i] = 'H';
      }
    }
    if( !foundC ){
      SafeFree(pFile->zHdr);
      pFile->zHdr = 0;
    }
  }

  /*
  ** If pFile->zSrc contains no 'c' or 'C' in its extension, it
  ** must be a header file.   In that case, we need to set the 
  ** PS_Interface flag.
  */
  pFile->flags |= PS_Interface;
  for(i=nSrc-1; i>0 && zSrc[i]!='.'; i--){
    if( zSrc[i]=='c' || zSrc[i]=='C' ){
      pFile->flags &= ~PS_Interface;
      break;
    }
  }

  /* Done! 
  */
  return pFile;
}