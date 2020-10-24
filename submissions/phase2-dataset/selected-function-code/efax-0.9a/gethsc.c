{
  int err=0, i ;
  if ( sresponse ( "+FHNG:", hsc ) || sresponse ( "+FHS:", hsc ) ) {
    if ( hsc && *hsc > 0 ) {
      err = msg ( "E2abnormal termination (code %d)", *hsc ) ;
      for ( i=0 ; c2msg[i].min >= 0 ; i++ ) {
	if ( *hsc >= c2msg[i].min && *hsc <= c2msg[i].max ) {
	  msg ( "E %s", c2msg[i].msg ) ;
	}
      }
      if ( perr && ! *perr ) {
	*perr = 2 ;
      }
    } else {
      err = 1 ;
    }
  }
  return err ;
}
