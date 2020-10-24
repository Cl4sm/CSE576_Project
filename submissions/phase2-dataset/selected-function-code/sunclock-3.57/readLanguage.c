void 
readLanguage()
{
    FILE *rc;           /* File pointer for rc file */
    char buf[1028];      /* Buffer to hold input lines */
    int i, j, k, l, m, p, tot;

    i = strlen(share_i18n);

    for (j=0; j<=1; j++) share_i18n[i+j-2] = tolower(language[j]);

    j = k = l = m = 0;
    tot = 2;
    if ((rc = fopen(share_i18n, "r")) != NULL) {
      while (fgets(buf, 1024, rc)) {
        if (buf[0] != '#') {
                p = strlen(buf) - 1;
                if (p>=0 && buf[p]=='\n') buf[p] = '\0';
                if (j<7) { strcpy(Day_name[j], buf); j++; } else
                if (k<12) { strcpy(Month_name[k], buf); k++; } else
                if (l<L_END) { 
                   StringReAlloc(&Label[l], buf); 
                   l++; 
                } else
                if (m<N_HELP) { 
		   StringReAlloc(&Help[m], buf);
                   m++; 
                } else break;
        }
      }
      strncpy(oldlanguage, language, 2);

      fclose(rc);
    } else {
        fprintf(stderr, 
             "Unable to open language in %s\n", share_i18n);
	strncpy(language, oldlanguage, 2);
    }
}
