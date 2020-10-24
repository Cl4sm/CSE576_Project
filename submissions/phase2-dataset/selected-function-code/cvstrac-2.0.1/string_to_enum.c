static void string_to_enum(const char *zEnum, const char *z){
  int i, j, n;
  int cnt = 1;
  char *zColor;
  char zName[50];
  char zDesc[200];

  db_execute("DELETE FROM enums WHERE type='%s'", zEnum);
  while( isspace(*z) ){ z++; }
  while( *z ){
    assert( !isspace(*z) );
    for(i=1; z[i] && !isspace(z[i]); i++){}
    n = i>49 ? 49 : i;
    memcpy(zName, z, n);
    zName[n] = 0;
    z += i;
    while( *z!='\n' && isspace(z[1]) ){ z++; }
    if( *z=='\n' || *z==0 ){
      strcpy(zDesc, zName);
      zColor = "";
    }else{
      int lastP1 = -1;
      int lastP2 = -1;
      z++;
      for(j=i=0; *z && *z!='\n'; z++){
        if( j<199 && (j==0 || !isspace(*z) || !isspace(zDesc[j-1])) ){
          zDesc[j++] = *z;
        }
        if( *z=='(' ){ lastP1 = j-1; }
        else if( *z==')' ){ lastP2 = j-1; }
        else if( !isspace(*z) ){ lastP2 = -1; }
      }
      zDesc[j] = 0;
      if( lastP2>lastP1 && lastP1>1 ){
        zColor = &zDesc[lastP1+1];
        zDesc[lastP2] = 0;
        zDesc[lastP1] = 0;
        j = lastP1;
        while( j>0 && isspace(zDesc[j-1]) ){ j--; }
        zDesc[j] = 0;
      }else{
        j = strlen(zDesc);
        while( j>0 && isspace(zDesc[j-1]) ){ j--; }
        zDesc[j] = 0;
        zColor = "";
      }
    }
    db_execute(
       "INSERT INTO enums(type,idx,name,value,color) "
       "VALUES('%s',%d,'%q','%q','%q')",
       zEnum, cnt++, zName, zDesc, zColor
    );
    while( isspace(*z) ) z++;
  }

  /* If the enums were updated such that one of the defaults was removed,
  ** choose a new default.
  */
  if( !strcmp(zEnum,"status") ){
    const char* zDefault = db_config("initial_state","new");
    char* z = db_short_query("SELECT name FROM enums "
                             "WHERE type='status' AND name='%q'", zDefault);
    if( z==0 || z[0]==0 ) {
      /* gone missing, update */
      db_execute(
        "REPLACE INTO config(name,value) "
        "VALUES('initial_state',(SELECT name FROM enums WHERE type='status'));"
      );
    }
  }else if( !strcmp(zEnum,"type") ){
    const char* zDefault = db_config("dflt_tkt_type","code");
    char* z = db_short_query("SELECT name FROM enums "
                             "WHERE type='type' AND name='%q'", zDefault);
    if( z==0 || z[0]==0 ) {
      /* gone missing, update */
      db_execute(
        "REPLACE INTO config(name,value) "
        "VALUES('dflt_tkt_type',(SELECT name FROM enums WHERE type='type'));"
      );
    }
  }
}