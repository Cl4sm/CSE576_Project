static int cvs_user_write(const char *zOmit){
  FILE *pswd;
  FILE *wrtr;
  FILE *rdr;
  char **az;
  int i;
  char *zFile;
  const char *zCvsRoot;
  const char *zUser = 0;
  const char *zWriteEnable;
  struct passwd *pw;

  /* If the "write_cvs_passwd" configuration option exists and is "no"
  ** (or at least begins with an 'n') then disallow writing to the
  ** CVSROOT/passwd file.
  */
  zWriteEnable = db_config("write_cvs_passwd","yes");
  if( zWriteEnable[0]=='n' ) return 1;

  /*
  ** Map the CVSTrac process owner to a real user id. This, presumably,
  ** will be someone with CVS read/write access.
  */
  pw = getpwuid(geteuid());
  if( pw==0 ) pw = getpwuid(getuid());
  if( pw ){
    zUser = mprintf("%s",pw->pw_name);
  }else{
    zUser = db_config("cvs_user_id","nobody");
  }

  zCvsRoot = db_config("cvsroot","");
  cvs_read_passwd_files(zCvsRoot);
  zFile = mprintf("%s/CVSROOT/passwd", zCvsRoot);
  pswd = fopen(zFile, "w");
  free(zFile);
  if( pswd==0 ) return 0;
  zFile = mprintf("%s/CVSROOT/writers", zCvsRoot);
  wrtr = fopen(zFile, "w");
  free(zFile);
  zFile = mprintf("%s/CVSROOT/readers", zCvsRoot);
  rdr = fopen(zFile, "w");
  free(zFile);
  az = db_query(
      "SELECT id, passwd, '%q', capabilities FROM user "
      "UNION ALL "
      "SELECT id, pswd, sysid, cap FROM tuser "
      "  WHERE id NOT IN (SELECT id FROM user) "
      "ORDER BY id", zUser);
  for(i=0; az[i]; i+=4){
    if( strchr(az[i+3],'o')==0 ) continue;
    if( zOmit && strcmp(az[i],zOmit)==0 ) continue;
    fprintf(pswd, "%s:%s:%s\n", az[i], az[i+1], az[i+2]);
    if( strchr(az[i+3],'i')!=0 ){
      if( wrtr!=0 ) fprintf(wrtr,"%s\n", az[i]);
    }else{
      if( rdr!=0 ) fprintf(rdr,"%s\n", az[i]);
    }
  }
  db_query_free(az);
  fclose(pswd);
  if( wrtr ) fclose(wrtr);
  if( rdr ) fclose(rdr);
  return 1;
}