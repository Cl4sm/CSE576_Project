int main(int n_arg, char *v_arg[])
  {

/***********************************************************/ 
#ifdef  __BORLANDC__
    _fmode = O_BINARY;
#endif
/***********************************************************/ 

    font_codep = strcpy(&font_code[0],FONT_CODE_2);
    verdateip = NULL;
    if (setvbuf(stdout,NULL,_IOLBF,0) != 0)
      { perror("setvbuf"); exit(1);}
    printf("\n%s ",v_arg[0]); printf("version 25.4.2000\n");
    if (n_arg >= 3) 
    { if (strcmp(v_arg[1],"-c") == 0)
      { font_codep = strcpy(&font_code[0],v_arg[2]);
        if (n_arg == 3)
        { printf("missing file name; exit\n");
          goto schluss;
        }
        else
        { hauptdateip = strcpy(&hauptdatei[0],v_arg[3]);
          if (n_arg >= 5)
          { verdateip = strcpy(&verdatei[0],v_arg[4]);}
        }
      }
      else
      { hauptdateip = strcpy(&hauptdatei[0],v_arg[1]);
        verdateip = strcpy(&verdatei[0],v_arg[2]);
      }
    }
    else
    if (n_arg == 1)
    { printf("missing file name; exit\n");
      goto schluss;
    }
    else 
    { hauptdateip = strcpy(&hauptdatei[0],v_arg[1]);
      if (n_arg == 3) {verdateip = strcpy(&verdatei[0],v_arg[2]);};
    };

    if ((strlen(hauptdateip) <= 4) || (strstr(hauptdateip,".dvi") == 0))
    { hauptdateip = strcat(hauptdateip,".dvi");
    };

    ausdateip = strncpy(&ausdatei[0],hauptdateip, strlen(hauptdateip)-4); 
    ausdateip = strcat(ausdateip,".hyp"); 

    filed1=open(hauptdateip,O_RDONLY);    
    if ((int)filed1 <= 0) {printf("\ninput file not found\n");goto schluss;}

    a_file=fopen(ausdateip,"w+");    
    if (a_file == NULL)
      {printf("\nwarning: could not open file %s for writing\n",ausdateip);};
    if (verdateip == NULL) {}
    else
    { ver_dat = fopen(verdateip,"r");    
      if (ver_dat == NULL) {printf("\nfile not found: %s\n",verdateip);}
    else
    { hck_p = 0;
      fscanf(ver_dat,"%s", & hck[hck_p][0]);
      while (feof(ver_dat) == 0)
      { hck_p++;
        if (hck_p >= laenge_hck)
        { printf("\nwarning: more than %d lines in file %s\n",
                  laenge_hck,verdateip);
          goto weiter;
        }
        fscanf(ver_dat,"%s", & hck[hck_p][0]);
      }
weiter:
     hck_max = hck_p;
    }
    }

    tr_gef = FALSE;

    printf("coding: %s\n",font_codep); 
    printf("same words are given only once\n\n"); 

    s_puffer_p = 0; s_p = 0; enthalten = FALSE;
    anzgef = 0; neu_max = 0; np = 0;
    seitep[s_p] = "";

    k =  e_inbyte();
    while (E_ENDFILE == FALSE)
      {
      if      (k == 0) {;}                         /* set_char_0            */
      else if (special_char(TRUE,k) == TRUE) {;}/* Ligatur, sz -- u. ---    */
      else if (k <= 126) {set_char_k(k);}       /* Ausgabe druckb. Zeichen  */
      else if (k == 127) {umlaut();}            /* Umlautzeichen wird zu "  */
      else if (k <= 131) {set_put(k-128);}      /* set1 bis set4            */
      else if (k == 132) {skip(8);}             /* set_rule                 */
      else if (k <= 136) {set_put(k-133);}      /* put1 bis put4            */
      else if (k == 137) {skip(8);}             /* put_rule                 */
      else if (k == 138) {;}                    /* nop (no operation)       */
      else if (k == 139) {bop(k);}              /* Seiten-Beginn            */
      else if (k == 140) {eop();}               /* Seiten-Ende              */
      else if (k == 141) {;}                    /* push                     */
      else if (k == 142) {;}                    /* pop                      */
      else if (k <= 146) {right(k);}            /* right1 bis right4        */
      else if (k == 147) {w0();}                /* w0                       */
      else if (k <= 151) {w(k);}                /* w1 bis w4                */
      else if (k == 152) {x0();}                /* x0                       */
      else if (k <= 156) {x(k);}                /* x1 bis x4                */
      else if (k <= 160) {down(k);}             /* down1 bis down4          */
      else if (k == 161) {y_0();}               /* y0                       */
      else if (k <= 165) {y(k);}                /* y1 bis y4                */
      else if (k == 166) {z0();}                /* z0                       */
      else if (k <= 170) {z(k);}                /* z1 bis z4                */
      else if (k <= 234) {;}                    /* Fontumschaltungen        */
      else if (k <= 238) {skip(k-234);}         /* fnt1 bis fnt4            */
      else if (k <= 242) {xxx(k);}              /* xxx1 bis xxx4            */
      else if (k <= 246) {fnt_def(k);}          /* Font-Definitionen        */
      else if (k == 247) {pre();}               /* Praeambel            */
      else if (k == 248) {break;}               /* Ueberlesen Post,Post_post*/
      ;   
      k =  e_inbyte();
    };

    close(filed1);

    printf("\nnumber of hyphenations:%5ld\n",gesamt);
    printf("already known/multiple:%5ld\n",anzgef);

    if (n_t1_ot1 > 0) 
    { printf("%d\n",n_t1_ot1);
      printf("warning: coding OT1 used, T1 may be correct\n"); 
    };
    if (a_file != NULL) {fclose(a_file);}
    else
      {printf("\nwarning: could not open file %s for writing\n\n",ausdateip);};
    exit(0);
schluss:  exit(1);
  }
