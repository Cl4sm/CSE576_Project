static void svn_delete_dir(
  int cn,
  int nRev,
  const char *zDir,
  int skipInsertFile
){
  int i;
  char **azTree;
  
  /* We need paths without leading '/' here since that is what we 
  ** already have in db.
  */
  while( zDir[0]=='/' ) zDir++;
  
  azTree = db_query(
    "SELECT fc.filename, fc.chngtype "
    "FROM filechng fc "
    "WHERE fc.filename LIKE '%q%%' AND fc.vers=( "
    "  SELECT MAX(vers) FROM filechng fc2 WHERE fc2.filename=fc.filename "
    ")  AND fc.chngtype<>2",
    zDir
  );
  
  if( azTree==0 ) return;

  for(i=0; azTree[i]; i+=2){
    int nFLen = strlen(azTree[i]);
    if( nFLen && azTree[i][nFLen-1]!='/' ){
      db_execute(
        "REPLACE INTO filechng(cn,filename,vers,prevvers,chngtype) "
        "VALUES(%d,'%q',%d,'%s',2);",
        cn, azTree[i], nRev, ( azTree[i+1][0]=='0' ) ? "" : azTree[i+1]
      );
    }
    if( !skipInsertFile ) insert_file(azTree[i], cn);
  }

  db_query_free(azTree);
}