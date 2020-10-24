{
  int err=0, m=EOP, yres, fVR, nVR  ;

  if ( nextipage ( f, dp ) )
    err = msg ( "E2 can't happen (rdpage: can't go to %s page)", 
	       dp ? "next" : "same" ) ;
  
  if ( ! err ) {

    yres = f->page->yres ;
    fVR = ( yres > (196+98)/2 ) ? 1 : 0 ;

    if ( local && yres ) {
      if ( local [ VR ] != fVR ) {
	local [ VR ] = fVR ;
	if ( changed ) *changed = 1 ;
      } else {
	if ( changed ) *changed = 0 ;
      }
    }

    if ( lastpage ( f ) ) {
      m = EOP ;
    } else {
      PAGE *p = f->page + 1 ;
      nVR = ( p->yres > (196+98)/2 ) ? 1 : 0  ;
      m = ( nVR != fVR ) ? EOM : MPS ;
    }

  }

  if ( ppm ) {
    *ppm = err ? EOP : m ;
  }

  return err ;
}
