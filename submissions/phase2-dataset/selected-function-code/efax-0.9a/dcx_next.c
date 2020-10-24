{
  int err=0 ;
  long thisp, nextp ;

  /* get this and next pages' offsets */

  fseek ( f->f, f->next, SEEK_SET ) ; 
  fread4 ( &thisp, f ) ;
  fread4 ( &nextp, f ) ;

  /* save address of next directory entry, if any */

  f->next = nextp ? f->next + 4 : 0 ;

  if ( ! thisp )
    err = msg ( "E2 can't happen (dcx_next)" ) ;
  else
    fseek ( f->f, thisp, SEEK_SET ) ;

  return err ? err : pcx_first ( f ) ;
}
