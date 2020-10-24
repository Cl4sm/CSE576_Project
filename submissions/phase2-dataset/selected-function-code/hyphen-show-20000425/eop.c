void eop(void)
  { char gef = FALSE;
    char * ttp, *ttap = NULL,*tgesp, *tcp, *terr;
    int ttapl,ntc;
    a_outimage();
    s_max = s_p-1;
    s_p = 0;
    test_neue_seite();
    while  (s_p <= s_max)
    { ttp = strip(seitep[s_p]);
      gef = gefunden(ttp);
      if (gef == TRUE)
      { ttap = wortanfang(ttp);
        ttapl = strlen(ttap);
        if (mitte-ttapl > 1) {pos1 = mitte-ttapl;} else pos1 = 1;
        s_p++; if (s_p > s_max) goto hinter_schl;
	if (ttapl < 2){gef = FALSE; goto schl;};
	if (ttapl == 2){if (ttap[0] == '}'){gef = FALSE; goto schl;};};
        test_neue_seite();
        ttp = strip(seitep[s_p]);
        terr = wortende(ttp);
        tgesp = strcat(ttap,terr);
        gesamt++;
        strcpy(&hh[0],tgesp);
        tcp = text_clear(&hh[0],&ntc);
        if (vergl_test(tcp) == FALSE)
        { if (vorschub == TRUE) {printf("\n"); np = 0; vorschub = FALSE;}
          if (a_file != NULL){f_set_pos(pos1+ntc);fprintf(a_file,"%s\n",tcp);};
          set_pos(pos1); printf("%s\n",tgesp);
          tr_gef = TRUE;
        }
        goto schl;
      }
      s_p++; if (s_p > s_max) goto hinter_schl;
      test_neue_seite();
schl: ;
    }
hinter_schl:
    if (gef == TRUE)
    { if (vergl_test(ttap) == FALSE)
      { if (vorschub == TRUE) {printf("\n"); np = 0; vorschub = FALSE;};
        if (a_file != NULL){f_set_pos(pos1); fprintf(a_file,"%s\n",ttap);};
        set_pos(pos1);  printf("%s\n",ttap);
      }
    }
    s_p = 0;
    seitep[s_p] = "";
  }
