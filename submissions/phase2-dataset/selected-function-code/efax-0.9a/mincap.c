int mincap ( cap local, cap remote, cap session )
{
  int err=0, i ;
  int msttab[2][8] = { { 0,1,3,3,5,5,7,7 } , { 0,1,1,3,3,5,5,7 } } ;

  printcap ( "local  ", local ) ;
  printcap ( "remote ", remote ) ;

  for ( i=0 ; i<NCAP && i!=ST && i !=BR ; i++ )
    session[i] = remote[i] < local[i] ? remote[i] : local[i] ;

  session[BR] = brindex[ remote[BR] ] < brindex[ local[BR] ] ?
    remote[BR] : local[BR] ;

  session[ST] = msttab [ session[VR] ] [ remote[ST] ] ;

  printcap ( "session", session ) ;

  if ( local[WD] != session[WD] || local[LN] > session[LN] || 
      local[DF] != session[DF] ) 
    err = msg ("W3incompatible local and remote capabilities" ) ;

  return err ;
}
