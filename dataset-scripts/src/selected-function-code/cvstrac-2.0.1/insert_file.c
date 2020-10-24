void insert_file(const char *zName, int cn){
  int i;
  char *zBase, *zDir;
  char *zToFree;
  int isDir = 0;
	int nLen;
  int isNewer;

  if( zName==0 || zName[0]==0 ) return;
	
  zToFree = zDir = mprintf("%s", zName);
  if( zDir==0 ) return;

  /* Subversion will pass directories too so we need to detect those early on.
  */
  nLen = strlen(zDir)-1;
  if( zDir[nLen]=='/' ){
    zDir[nLen--] = 0;
    isDir = 1;
  }
	
  while( zDir && zDir[0] ){
    for(i=nLen; i>0 && zDir[i]!='/'; i--){}
    if( i==0 ){
      zBase = zDir;
      zDir = "";
    }else{
      zDir[i] = 0;
      zBase = &zDir[i+1];
    }
    isNewer = db_exists(
      "SELECT 1 FROM file WHERE dir='%q' AND base='%q' AND lastcn>%d",
      zDir, zBase, cn
    );
    if( isNewer ) break;
    db_execute(
      "REPLACE INTO file(isdir, base, dir, lastcn) "
      "VALUES(%d,'%q','%q',%d)",
      isDir, zBase, zDir, cn);
    isDir = 1;
    zName = zDir;
  }
  free(zToFree);
}