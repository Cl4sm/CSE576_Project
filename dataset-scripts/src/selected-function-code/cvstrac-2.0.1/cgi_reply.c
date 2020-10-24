void cgi_reply(void){
  FILE *log;
  if( iReplyStatus<=0 ){
    iReplyStatus = 200;
    zReplyStatus = "OK";
  }

  if( iReplyStatus==200 && check_cache_control() ) {
    /* change the status to "unchanged" and we can skip sending the
    ** actual response body. Obviously we only do this when we _have_ a
    ** body (code 200).
    */
    iReplyStatus = 304;
    zReplyStatus = "Not Modified";
  }

  if( fullHttpReply ){
    printf("HTTP/1.0 %d %s\r\n", iReplyStatus, zReplyStatus);
    printf("Date: %s\r\n", cgi_rfc822_datestamp(time(0)));
    printf("Connection: close\r\n");
  }else{
    printf("Status: %d %s\r\n", iReplyStatus, zReplyStatus);
  }

  if( zExtraHeader ){
    printf("%s", zExtraHeader);
  }

  if( g.isConst ){
    /* constant means that the input URL will _never_ generate anything
    ** else. In the case of attachments, the contents won't change because
    ** an attempt to change them generates a new attachment number. In the
    ** case of most /getfile calls for specific versions, the only way the
    ** content changes is if someone breaks the SCM. And if that happens, a
    ** stale cache is the least of the problem. So we provide an Expires
    ** header set to a reasonable period (default: one week).
    */
    time_t expires = time(0) + atoi(db_config("constant_expires","604800"));
    printf( "Expires: %s\r\n", cgi_rfc822_datestamp(expires));
  }

  if( g.isAnon ){
    printf("Cache-control: public\r\n");
  }else{
    /* Content intended for logged in users should only be cached in
    ** the browser, not some shared location.
    */
    printf("Cache-control: private\r\n");
  }

#if CVSTRAC_I18N
  printf( "Content-Type: %s; charset=%s\r\n", zContentType, nl_langinfo(CODESET));
#else
  printf( "Content-Type: %s; charset=ISO-8859-1\r\n", zContentType);
#endif

  if( iReplyStatus != 304 ) {
    printf( "Content-Length: %d\r\n", nUsedTxt );
  }
  printf("\r\n");
  if( zTxt && iReplyStatus != 304 ){
    fwrite(zTxt, 1, nUsedTxt, stdout);
  }
  if( zLogFile && (log = fopen(zLogFile,"a"))!=0 ){
    time_t now;
    struct tm *pTm;
    const char *zPath;
    const char *zAddr;
    struct tms sTms;
    double rScale;
    char zDate[200];

    if( zLogArg==0 ) zLogArg = "*";
    zPath = getenv("REQUEST_URI");
    if( zPath==0 ) zPath = "/";
    zAddr = getenv("REMOTE_ADDR");
    if( zAddr==0 ) zAddr = "*";
    time(&now);
    pTm = localtime(&now);
    strftime(zDate, sizeof(zDate), "%Y-%m-%d %H:%M:%S", pTm);
    fprintf(log, "%s %s %s %d %s", zDate, zAddr, zPath, iReplyStatus,zLogArg);
    times(&sTms);
    rScale = 1.0/(double)sysconf(_SC_CLK_TCK);
    fprintf(log, " %g %g %g %g\n",
      rScale*sTms.tms_utime,
      rScale*sTms.tms_stime,
      rScale*sTms.tms_cutime,
      rScale*sTms.tms_cstime);
    fclose(log);
  }
}