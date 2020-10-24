{
  int err = 0, i, nc ;
  char buf [ MAXLINELEN ] ;

  f->page->offset = ftell ( f->f ) ;
  f->page->format = P_TEXT ;

  nc = ( f->page->w - f->lmargin ) / f->charw ;

  if ( nc > MAXLINELEN ) 
    nc = MAXLINELEN ;

  for ( i = 0 ; i < f->pglines && fgets ( buf, nc, f->f ) ; i++ ) ;

  f->next = ! feof(f->f) ? ftell ( f->f ) : 0 ;

  err = ferror(f->f) ? 2 : 0 ;

  return err ;
}
