{
  int i ;

  if ( ! tabinit ) {

    /* undefined codes */

    addcode ( tw1,  0, 9, 0, 1, tw1 ) ;
    addcode ( tw2,  0, 9, 0, 1, tw1 ) ;
    addcode ( tb1,  0, 9, 0, 1, tw1 ) ;
    addcode ( tb2,  0, 9, 0, 1, tw1 ) ;
    addcode ( fill, 0, 9, 0, 1, tw1 ) ;
  
    /* fill and EOL */

    addcode ( tw1, 0, 0, 0, 4, tw2 ) ;
    addcode ( tw2, 0, 2, 0, 7, fill ) ;
    addcode ( tb1, 0, 0, 0, 4, tb2 ) ;
    addcode ( tb2, 0, 2, 0, 7, fill ) ;

    addcode ( fill, 0, 0, 0, 9, fill ) ;
    for ( i=0 ; i<=8 ; i++ )
      addcode ( fill, 1, i, -1, 9-i, tw1 ) ;

    /* white and black runs */
    
    init1dtab ( wtab, tw1, tw2, tb1 ) ;
    init1dtab ( btab, tb1, tb2, tw1 ) ;

    tabinit=1 ;
  }

  /* initialize decoder to starting state */

  d->x = 0 ;
  d->shift = -9 ;
  d->tab = tw1 ;
  d->eolcnt = 0 ;
}
