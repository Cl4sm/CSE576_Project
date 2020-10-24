char *nbsdecrypt(cpt,key,result)
  char *cpt,*key;
  char *result;
{
	char *s;
	char c,oldbuf[20];
	result[0] = 0;
	strcpy(oldbuf,key);
	while(*cpt){
		for(s = cpt;*s && *s != '$';s++);
		c = *s;
		*s = 0;
		strcpy(oldbuf,nbs8decrypt(cpt,oldbuf));
		strcat(result,oldbuf);
		if(c == 0)break;
		cpt = s + 1;
		}
	return(result);
}
