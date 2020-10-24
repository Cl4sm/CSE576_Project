{
#define CNAMEFMT "<0x%02x>"
#define CNAMELEN 6+1
  static char *cnametab [ 256 ] = { /* character names */
  "<NUL>","<SOH>","<STX>","<ETX>", "<EOT>","<ENQ>","<ACK>","<BEL>",
  "<BS>", "<HT>", "<LF>", "<VT>",  "<FF>", "<CR>", "<SO>", "<SI>", 
  "<DLE>","<XON>","<DC2>","<XOFF>","<DC4>","<NAK>","<SYN>","<ETB>",
  "<CAN>","<EM>", "<SUB>","<ESC>", "<FS>", "<GS>", "<RS>", "<US>" } ;
  static char names[ (127-32)*2 + 129*(CNAMELEN) ] ;
  char *p=names ;
  static int i=0 ;
    
  if ( ! i ) {
    for ( i=32 ; i<256 ; i++ ) {
      cnametab [ i ] = p ;
      sprintf ( p, i<127 ? "%c" : CNAMEFMT, i ) ;
      p += strlen ( p ) + 1 ;
    }
  }

  return cnametab [ c ] ;
} 
