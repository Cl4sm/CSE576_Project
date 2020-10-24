  {
/* Sonderzeichen fuer "von" "bis" (--) und Gedankenstrich (---)
   sollen in der Form angezeigt werden.
*/
    if ((char) k == '{') {a_outchar('-'); a_outchar('-'); return TRUE;}
    if ((char) k == '|') {a_outchar('-'); a_outchar('-'); a_outchar('-');
                          return TRUE;}
    if (strcmp(font_code, FONT_CODE_1) == 0)
    {
      if (opcode == FALSE){if (k > 127) { n_t1_ot1 = n_t1_ot1 + 1;
					  return FALSE;}};
      if      (k <= 10) {a_outchar('^'); a_outchar('^'); return TRUE;}
      else if (k == 11) {a_outchar('f'); a_outchar('f'); return TRUE;}
      else if (k == 12) {a_outchar('f'); a_outchar('i'); return TRUE;}
      else if (k == 13) {a_outchar('f'); a_outchar('l'); return TRUE;}
      else if (k == 14) {a_outchar('f'); a_outchar('f'); a_outchar('i');
                         return TRUE;}
      else if (k == 15) {a_outchar('f'); a_outchar('f'); a_outchar('l');
                         return TRUE;}
      else if (k <= 24) {a_outchar('^'); a_outchar('^'); return TRUE;}
      else if (k == 25) {a_outchar('"'); a_outchar('s'); return TRUE;}
      else if (k <= 31) {a_outchar('^'); a_outchar('^'); return TRUE;}
      else return FALSE;
    }
    else if (strcmp(font_code,  FONT_CODE_2) == 0)
    { if      (k <= 15) {a_outchar('^'); a_outchar('^'); return TRUE;}
      else if (k == 16) {a_outchar('"'); return TRUE;}
      else if (k == 17) {a_outchar('"'); return TRUE;}
      else if (k <= 26) {a_outchar('^'); a_outchar('^'); return TRUE;}
      else if (k == 27) {a_outchar('f'); a_outchar('f'); return TRUE;}
      else if (k == 28) {a_outchar('f'); a_outchar('i'); return TRUE;}
      else if (k == 29) {a_outchar('f'); a_outchar('l'); return TRUE;}
      else if (k == 30) {a_outchar('f'); a_outchar('f'); a_outchar('i');
                         return TRUE;}
      else if (k == 31) {a_outchar('f'); a_outchar('f'); a_outchar('l');
                         return TRUE;}
      else if (k == 255) {a_outchar((char)223); return TRUE;}
      else return FALSE;
    }
    else return FALSE;
  }
