static void cap_and(sqlite3_context *context, int argc, sqlite3_value **argv){
  int i, j;
  const char *z;
  char set[26];
  char zResult[27];
  if( argc!=2 ) return;
  for(i=0; i<26; i++) set[i] = 0;
  z = (const char*)sqlite3_value_text(argv[0]);
  if( z ){
    for(i=0; z[i]; i++){
      if( z[i]>='a' && z[i]<='z' ) set[z[i]-'a'] = 1;
    }
  }
  z = (const char*)sqlite3_value_text(argv[1]);
  if( z ){
    for(i=0; z[i]; i++){
      if( z[i]>='a' && z[i]<='z' && set[z[i]-'a']==1 ) set[z[i]-'a'] = 2;
    }
  }
  for(i=j=0; i<26; i++){
    if( set[i]==2 ) zResult[j++] = i+'a';
  }
  zResult[j] = 0;
  sqlite3_result_text(context, zResult, -1, SQLITE_TRANSIENT);
}