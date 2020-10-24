static void cvs_read_passwd_files(const char *zCvsRoot){
  FILE *f;
  char *zFile;
  char *zPswd, *zSysId;
  int i;
  char zLine[2000];
  

  db_execute(
    "CREATE TEMP TABLE tuser(id UNIQUE ON CONFLICT IGNORE,pswd,sysid,cap);"
    "CREATE TEMP TABLE treadonly(id UNIQUE ON CONFLICT IGNORE);"
  );
  if( zCvsRoot==0 ){
    zCvsRoot = db_config("cvsroot","");
  }
  zFile = mprintf("%s/CVSROOT/passwd", zCvsRoot);
  f = fopen(zFile, "r");
  free(zFile);
  if( f ){
    while( fgets(zLine, sizeof(zLine), f) ){
      remove_newline(zLine);
      for(i=0; zLine[i] && zLine[i]!=':'; i++){}
      if( zLine[i]==0 ) continue;
      zLine[i++] = 0;
      zPswd = &zLine[i];
      while( zLine[i] && zLine[i]!=':' ){ i++; }
      if( zLine[i]==0 ){
        zSysId = zLine;
      }else{
        zLine[i++] = 0;
        zSysId = &zLine[i];
      }
      db_execute("INSERT INTO tuser VALUES('%q','%q','%q','io');",
         zLine, zPswd, zSysId);
    }
    fclose(f);
  }
  zFile = mprintf("%s/CVSROOT/readers", zCvsRoot);
  f = fopen(zFile, "r");
  free(zFile);
  if( f ){
    while( fgets(zLine, sizeof(zLine), f) ){
      remove_newline(zLine);
      db_execute("INSERT INTO treadonly VALUES('%q');", zLine);
    }
    fclose(f);
  }
  zFile = mprintf("%s/CVSROOT/writers", zCvsRoot);
  f = fopen(zFile, "r");
  free(zFile);
  if( f ){
    db_execute("INSERT INTO treadonly SELECT id FROM tuser");
    while( fgets(zLine, sizeof(zLine), f) ){
      remove_newline(zLine);
      db_execute("DELETE FROM treadonly WHERE id='%q';", zLine);
    }
    fclose(f);
  }
  db_execute(
    "UPDATE tuser SET cap='o' WHERE id IN (SELECT id FROM treadonly);"
  );
}