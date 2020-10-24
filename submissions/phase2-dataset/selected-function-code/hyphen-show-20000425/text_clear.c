char* text_clear(char * str, int* p)
  { unsigned n; char cc;
    n = 0; *p = 0;
    while (strlen(str) > 0)
    { cc = (char) str[strlen(str)-1];
      if (zugelassen(FALSE,cc) == FALSE) 
      { str[strlen(str)-1] = (char) 0;}
      else goto trail;
    }
trail:
    while (n < strlen(str))
    { cc = (char) str[n];
      if (zugelassen(TRUE,cc) == FALSE) {n++;}
      else goto fin;
    }
fin:
    *p =  n;
    memset(&hh1[0],(char)0,laenge_puf);
    return strncpy(&hh1[0], &str[n], strlen(&str[n]));
  }
