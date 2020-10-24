{
  int err=0 ;

  fseek ( f->f, 2, SEEK_SET ) ;

  if ( ! ( f->page->w = pbmdim ( f ) ) || ! ( f->page->h = pbmdim ( f ) ) ) {
    err = msg ( "E2 EOF or 0 dimension in PBM header" ) ;
  } else if ( f->page->w % 8 ) {
    err = msg ( "E2 PBM width must be multiple of 8" ) ;
  } else {
    msg ( "F read %dx%d PBM header", f->page->w, f->page->h ) ;
  }

  f->page->offset = ftell ( f->f ) ;
  f->page->format = P_PBM ;
  f->next = 0 ;

  return err ;
}
