static void svn_insert_copied_files(
  int cn,
  int nRev,
  const char *zNewDir, 
  const char *zOldDir,
  int skipInsertFile
){
  int i;
  size_t nLen;
  char **azTree;
  char *zTmp;
  
  /* We need paths without leading '/' here since that is what we 
  ** already have in db.
  */
  while( zNewDir[0]=='/' ) zNewDir++;
  while( zOldDir[0]=='/' ) zOldDir++;
  
  /* zOldDir must end with '/' if it's not empty string.
  */
  zTmp = (strlen(zOldDir)==0) ? mprintf("") : mprintf("%s/", zOldDir);
  if( zTmp==0 ) common_err("Out of memory");
  nLen = strlen(zTmp);
  
  /* This query should return only files that are not deleted.
  */
  azTree = db_query(
    "SELECT fc.filename, fc.chngtype "
    "FROM filechng fc "
    "WHERE fc.filename LIKE '%q%%' AND fc.vers=( "
    "  SELECT MAX(vers) FROM filechng fc2 WHERE fc2.filename=fc.filename "
    ")  AND fc.chngtype<>2",
    zTmp
  );
  free(zTmp);
  
  if( azTree==0 ) return;

  for(i=0; azTree[i]; i+=2){
    char *zFile;
    int nFLen;
    if( strlen(zNewDir)>0 ){
      zFile = mprintf("%s/%s", zNewDir, &(azTree[i])[nLen]);
    }else{
      zFile = mprintf("%s", &(azTree[i])[nLen]);
    }
    if( zFile==0 ) common_err("Out of memory");
    nFLen = strlen(zFile);
    if( nFLen && zFile[nFLen-1]!='/' ){
      db_execute(
        "REPLACE INTO filechng(cn,filename,vers,prevvers,chngtype) "
        "VALUES(%d,'%q',%d,'%s',1);",
        cn, zFile, nRev, ( azTree[i+1][0]=='0' ) ? "" : azTree[i+1]
      );
    }
    if( !skipInsertFile ) insert_file(zFile, cn);
    free(zFile);
  }
  db_query_free(azTree);
}