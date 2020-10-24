{
  enum connectmode mode = NONE ;
  int x=0 ;

  if ( c1 && sresponse ( "CONNECT", &x ) ) {
    mode = x ? DATAMODE : FAXMODE ;
  }

  if ( !c1 && sresponse ( "OK", 0 ) ) {
    mode = FAXMODE ;
  } 

  if ( !c1 && ( sresponse ( "CONNECT", &x ) || sresponse ( "+FDM", 0 ) ) ) {
    mode = DATAMODE ;
  } 

  if ( sresponse ( "DATA", 0 ) || sresponse ( "CONNECT DATA", 0 ) ) {
    mode = DATAMODE ;
    sresponse ( "CONNECT", &x ) ;
  }

  if ( sresponse ( "FAX", 0 ) || sresponse ( "+FCO", 0 ) ) {
    mode = FAXMODE ;
  }

  if ( sresponse ( "VCON", 0 ) ) {
    mode = VOICEMODE ;
  }
  
  if ( gethsc ( hsc, 0 ) ) {
    mode = NONE ;
  }

  if ( DATAMODE && x ) *crate = x ;

  return mode ;
}
