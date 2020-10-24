static int git_read_refs(const char *zGitDir,const char *zType){
  DIR *dir;
  struct dirent *ent;
  int nCommits = 0;

  char *zFile = mprintf("%s/refs/%s", zGitDir, zType);
  dir = opendir( zFile );
  free(zFile);
  if( dir==NULL ) return 0;

  while( 0!=(ent=readdir(dir)) ){
    char zObject[100];
    char *zContents;
    char *zFile;
    char **azRef;
    struct stat statbuf;
    int cn = 0;

    if( ent->d_name[0]=='.' ) continue;

    zFile = mprintf("%s/refs/%s/%s", zGitDir, zType, ent->d_name);
    if( zFile==0 ) continue;

    if( stat(zFile, &statbuf) ){
      /* Can't read the file, skip */
      free(zFile);
      continue;
    }

    azRef = db_query("SELECT object,cn,seen FROM %s WHERE name='%q'",
                     zType, ent->d_name);

    if( azRef && azRef[0] && azRef[1] && azRef[2] ){
      if( statbuf.st_mtime<=atoi(azRef[2]) ){
        /* file hasn't been modified since last time we looked at it */
        db_query_free(azRef);
        free(zFile);
        continue;
      }
      cn = atoi(azRef[1]);  /* don't need to lose this... */
    }

    zContents = common_readfile( zFile );
    free(zFile);
    if(zContents==0) continue;
    zObject[0] = 0;
    sscanf(zContents,"%50[0-9a-fA-F]",zObject);
    free(zContents);
    if( zObject[0]==0 ) continue;

    /* update the seen time... We'll be updating cn later, after we actually
    ** read any new stuff
    */
    db_execute("REPLACE INTO %s(name,cn,object,seen) VALUES('%q',%d,'%q',%d)",
               zType, ent->d_name, cn, zObject, statbuf.st_mtime);

    if( azRef && azRef[0] && !strcmp(zObject,azRef[0]) ){
      /* contents of the ref haven't changed */
      db_query_free(azRef);
      continue;
    }

    /* Fill out the temporary ci table with any changes. */
    nCommits += git_ingest_commit_tree(zGitDir,zObject);

    db_query_free(azRef);
  }
  closedir(dir);

  return nCommits;
}