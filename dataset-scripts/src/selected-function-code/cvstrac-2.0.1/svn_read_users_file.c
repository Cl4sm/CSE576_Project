static void svn_read_users_file(const char *zSvnRoot){
  FILE *f;
  int i, bInSection=0;
  char *zFile, *zKey, *zValue;
  char zLine[2000];
  char *zAnonAccess, *zAuthAccess;
  /* We set these to Subversion defaults in case they are not defined 
  ** explicitly in .conf file.
  */
  zAnonAccess = "o"; /* TODO: use this to set anon user's perms */
  zAuthAccess = "io";
  
  db_execute(
    "CREATE TEMP TABLE tuser(id UNIQUE ON CONFLICT IGNORE,pswd,sysid,cap);"
  );
  if( zSvnRoot==0 ){
    zSvnRoot = db_config("cvsroot","");
  }
  zFile = mprintf("%s/conf/svnserve.conf", zSvnRoot);
  f = fopen(zFile, "r");
  free(zFile);
  /*
  ** This is how Subversion .conf file should look like:
  ** 
  ** # This is commnet
  ** [general] # Section name
  ** anon-access = read # What can anon users do
  ** auth-access = write # What can auth users do
  ** password-db = passwd # Absolute or relative location of users file
  ** realm = My First Repository # This is of no intrest to us
  ** [some_other_section]
  */
  if( f ){
    while( fgets(zLine, sizeof(zLine), f) ){
      remove_newline(zLine);
      for(i=0; zLine[i] && isspace(zLine[i]); i++){}
      if( zLine[i]==0 || zLine[i]=='#' ) continue;
      if( zLine[i]=='[' ){
        /* If we are already in section, then this marks the end of our section.
        ** If not, this might be the start of our section.
        */
        if( bInSection ){
          bInSection = 0;
          break;
        } else if( strncmp(&zLine[i], "[general]", 9)==0 ){
          bInSection = 1;
          continue;
        }
      }
      
      /* If we're not in our section there is nothing to look for in this line.
      ** TODO: I made an assumption here that all keys for key-value pairs in 
      ** .conf files have to start with alnum char. Check if this really 
      ** is the case.
      */
      if( !bInSection || !isalnum(zLine[i]) ) continue;
      
      /* Extract key-value pair */
      zKey = &zLine[i];
      for(; zLine[i] && !isspace(zLine[i]); i++){}
      zLine[i++] = 0;
      for(; zLine[i] && isspace(zLine[i]); i++){}
      if( zLine[i++]!='=' ) continue; /* Not valid key-value pair */
      for(; zLine[i] && isspace(zLine[i]); i++){}
      zValue = &zLine[i];
      for(; zLine[i] && !isspace(zLine[i]); i++){}
      zLine[i] = 0; /* Make sure we rtrim our value */
      
      /* We handel each key differently */
      if( strcmp(zKey, "anon-access")==0 ){
        if( strcmp(zValue, "none")==0 ){ zAnonAccess=""; continue; }
        if( strcmp(zValue, "read")==0 ){ zAnonAccess="o"; continue; }
        if( strcmp(zValue, "write")==0 ){ zAnonAccess="io"; continue; }
      }
      
      if( strcmp(zKey, "auth-access")==0 ){
        if( strcmp(zValue, "write")==0 ){ zAuthAccess="io"; continue; }
        if( strcmp(zValue, "read")==0 ){ zAuthAccess="o"; continue; }
        if( strcmp(zValue, "none")==0 ){ zAuthAccess=""; continue; }
      }
      
      if( strcmp(zKey, "password-db")==0 ){
        if( zValue[0]=='/' ){
          /* We've got absolute path, no problem here */
          zFile = mprintf("%s", zValue);
          continue;
        } else {
          /* We've got relative path */
          zFile = mprintf("%s/conf/%s", zSvnRoot, zValue);
          continue;
        }
      }
    }
    fclose(f);
  }
  
  /* If we didn't find path to users file, exit */
  if( !zFile || !zFile[0] ) return;
  
  f = fopen(zFile, "r");
  free(zFile);
  
  /* Read username->password pairs from file and store them in db
  ** Passwords are stored in clear text.
  */
  if( f ){
    bInSection = 0;
    while( fgets(zLine, sizeof(zLine), f) ){
      char zBuf[3];
      char zSeed[100];
      const char *z;
      remove_newline(zLine);
      for(i=0; zLine[i] && isspace(zLine[i]); i++){}
      if( zLine[i]==0 || zLine[i]=='#' ) continue;
      if( zLine[i]=='[' ){
        /* If we are already in section, then this marks the end of our section.
        ** If not, this might be the start of our section.
        */
        if( bInSection ){
          bInSection = 0;
          break;
        } else if( strncmp(&zLine[i], "[users]", 7)==0 ){
          bInSection = 1;
          continue;
        }
      }
      
      /* If we're not in our section there is nothing to look for in this line.
      ** TODO: I made an assumption here that all usernames have to start 
      ** with alnum char. Check if this really is the case.
      */
      if( !bInSection || !isalnum(zLine[i]) ) continue;
      
      /* Each line here is key-value pair, or in our case uname-passwd pair */
      zKey = &zLine[i];
      for(; zLine[i] && !isspace(zLine[i]); i++){}
      zLine[i++] = 0;
      for(; zLine[i] && isspace(zLine[i]); i++){}
      if( zLine[i++]!='=' ) continue; /* Not valid key-value pair */
      for(; zLine[i] && isspace(zLine[i]); i++){}
      zValue = &zLine[i];
      for(; zLine[i] && !isspace(zLine[i]); i++){}
      zLine[i] = 0; /* Make sure we rtrim our value */
      
      /* We need to encrypt passwords */
      bprintf(zSeed,sizeof(zSeed),"%d%.20s",getpid(),zKey);
      z = crypt(zSeed, "aa");
      zBuf[0] = z[2];
      zBuf[1] = z[3];
      zBuf[2] = 0;
          
      db_execute("INSERT INTO tuser VALUES('%q','%q','','%s');",
         zKey, crypt(zValue, zBuf), zAuthAccess);
    }
    fclose(f);
  }
}