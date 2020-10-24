static void ticket_attach(
  time_t date,          /* date/time of the attachment */
  int attachn,          /* attachment number */
  size_t size,          /* size, in bytes, of the attachment */
  const char *zUser,    /* username that created it */
  const char *zDescription,    /* description of the attachment */
  const char *zFilename /* name of attachment file */
){
  char zDate[100];
  struct tm *pTm;
  pTm = localtime(&date);
  strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M:%S", pTm);
  @ <li> Attachment 
  @ <a href="attach_get/%d(attachn)/%h(zFilename)">%h(zFilename)</a>
  @ %d(size) bytes added by %h(zUser) on %h(zDate).
  if( zDescription && zDescription[0] ){
    @ <br>
    output_formatted(zDescription,NULL);
    @ <br>
  }
  if( ok_to_delete_attachment(date, zUser) ){
    @ [<a href="attach_del?atn=%d(attachn)">delete</a>]
  }
  @ </li>
}