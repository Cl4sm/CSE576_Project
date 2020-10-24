str2numval(s, val, max)
char *s;
int *val;
int max;
{
int i, j, l;
char *ptr;
 
    l = strlen(s);

    j = 0;
    ptr = s;
    for (i=0; i<=l; i++) {
        if (s[i] == '|' || i == l) {
	   s[i] = '\0';
	   if (j>=city_cat) break;
           val[j] = atoi(ptr);
	   if (max>0 && val[j]>max) val[j] = max;
	   ++j;
	   ptr = s+i+1;
           if (i<l) s[i] = '|';
	}
    }

    for (i=j; i<city_cat; i++) val[i] = val[j-1];
}
