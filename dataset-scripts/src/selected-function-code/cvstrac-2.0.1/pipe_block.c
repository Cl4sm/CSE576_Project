static void pipe_block(
  const char *zPipeline,
  const char *zBlock,
  int lenBlock
){
  char zFile[PATH_MAX];
  char *zB = mprintf("%.*s",lenBlock,zBlock);
  FILE *fin = NULL;
  char *zP;

  /* Doing this without a temporary file is a bit nasty because of
  ** potential deadlocks. It _can_ be done if you want to fight with
  ** pipe(2) and stuff, but CVSTrac already has a write_to_temp() function
  ** so we might as well be lazy and use it. Note that if lenBlock==0
  ** we can just skip out using /dev/null.
  */

  zFile[0] = 0;
  if( lenBlock==0 ) {
    /* In case the program takes arguments from the command line, we
    ** don't want to just treat it as a no-op. So pipe in /dev/null.
    */
    zP = mprintf( "%s </dev/null", zPipeline );
  }else if( !write_to_temp( zB, zFile, sizeof(zFile) ) ){
    zP = mprintf( "%s <%s", zPipeline, zFile );
  }else{
    if( zB ) free(zB);
    return;
  }

  /* Block has been written, free so we don't forget later
  */
  if( zB ) free(zB);

  fin = popen(zP,"r");
  free(zP);

  /* HTML scrubbing doesn't work effectively on just individual lines. We
  ** really need to feed in the entire buffer or we're vulnerable to all
  ** sorts of whitespace stupidity.
  */
  zP = common_readfp(fin);
  if( zP ){
    output_restricted_html(zP, strlen(zP));
    free( zP );
  }

  if( fin ){
    pclose(fin);
  }

  if( zFile[0] ){
    unlink(zFile);
  }
}