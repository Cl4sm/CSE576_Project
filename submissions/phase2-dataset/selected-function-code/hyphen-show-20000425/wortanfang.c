char *wortanfang(char *str)
  { char ht[2] = {(char)0, (char)0};
    int n,k;
    n = strlen(str);

    k = 0;
    while ((str[k] == ' ') && (k <= n)) k++;
    memset(&stra[0], (char)0,laenge_puf);
    strncpy(&stra[0], &str[k],n-k);
    if (k == n) goto fin;
    n = n-k; k = n;
    ht[0] = (char)0;
    if (n <= 2) 
    { if (stra[0] == '-') {goto fin;}
      else {ht[0] = '-';}
    }
/* wenn sich \emph{Textwert}- am Ende einer Zeile ergibt, dann steht 
                >Textwert -< in .dvi-Datei, deshalb:
*/
    if (stra[n-2] == ' ') {stra[n-2] = '}';}

    while ((k>0) && (stra[k-1] != ' '))
    { if (k==0) {goto fin;}
      k--;
    }
fin:
   memset(&wortanf[0], (char)0,laenge_puf);
   return strcat(strncpy(&wortanf[0],&stra[k],n-k),&ht[0]);
  } 
