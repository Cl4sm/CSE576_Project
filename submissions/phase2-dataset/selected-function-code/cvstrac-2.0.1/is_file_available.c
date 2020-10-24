int is_file_available(const char *zFile){
  if( g.scm.pxIsFileAvailable ){
    return g.scm.pxIsFileAvailable(zFile);
  }else{
    /*
    ** Other SCMs can makes sense of the database. However, this only makes
    ** sense when trees are independent.
    */
    char *zLastChng = db_short_query(
      "SELECT chngtype FROM filechng WHERE filename='%q' ORDER BY cn DESC;",
      zFile
    );
    
    if( zLastChng && zLastChng[0] && atoi(zLastChng)!=2 ){
      free(zLastChng);
      return 1; /* File is available */
    }
    if(zLastChng) free(zLastChng);
  }
  return 0;
}