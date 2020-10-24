int cvs_previous_version(char *zVers,const char *zFile){
  size_t sz = strlen(zVers)+1;
  while( zVers[0] ) {
    int j, x;
    int n = strlen(zVers);
    for(j=n-2; j>=0 && zVers[j]!='.'; j--){}
    if(j<0) break;
    j++;
    x = atoi(&zVers[j]);
    if( x>1 ){
      bprintf(&zVers[j],sz-j,"%d",x-1);
    }else{
      for(j=j-2; j>0 && zVers[j]!='.'; j--){}
      if(j<0) break;
      zVers[j] = 0;
    }

    if(zVers[0]) {
      if( zFile ){
        if( db_exists("SELECT 1 FROM filechng "
                      "WHERE filename='%q' AND vers='%q'",
                      zFile,zVers) ){
          return 1;
        }
      }else{
        /* can't determine if it's good or bad, so assume good */
        break;
      }
    }
  }

  return zVers[0];
}