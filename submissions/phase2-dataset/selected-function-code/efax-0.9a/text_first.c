{
  static faxfont defaultfont ;

  if ( ! f->font ) {
    /* use default font scaled 2X, 1 inch margin, 66 lines/page */
    f->font = &defaultfont ;
    readfont ( 0, f->font ) ; 
    if ( ! f->charw ) f->charw = 2 * f->font->w ;
    if ( ! f->charh ) f->charh = 2 * f->font->h ;
    if ( ! f->lmargin ) f->lmargin = 204 ;
    if ( ! f->pglines ) f->pglines = DEFPGLINES ;
  }
  /* if not set, take default values from font  */
  if ( ! f->charw ) f->charw = f->font->w ;
  if ( ! f->charh ) f->charh = f->font->h ;
  if ( ! f->lmargin ) f->lmargin = 0 ;
  if ( ! f->pglines ) f->pglines = f->page->h / f->charh - 6 ;
  
  return text_next ( f ) ; 
}
