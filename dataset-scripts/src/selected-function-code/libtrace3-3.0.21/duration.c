static char *duration(double ts)
{
	static char ret[1024];
	char tmp[1024];
	ret[0]='\0';
	if (ts == 0) 
		return "0 seconds";
	if (ts>=24*60*60) {
		snprintf(ret,sizeof(ret),"%i days",(int)(ts/(24*60*60)));
		ts-=(int)(ts/(24*60*60))*24*60*60;
	}
	if (ts>=60*60) {
		snprintf(tmp,sizeof(tmp),"%s%i hours",
				ret[0]?", ":"",
				(int)ts/(60*60));
		strncat(ret,tmp, 1024 - strlen(ret) - 1);
		ts-=(int)(ts/(60*60))*60*60;
	}
	if (ts>=60) {
		snprintf(tmp,sizeof(tmp),"%s%i minutes",
				ret[0]?", ":"",
				(int)ts/60);
		strncat(ret,tmp, 1024 - strlen(ret) - 1);
		ts-=(int)(ts/60)*60;
	}
	if (ts>0) {
		snprintf(tmp,sizeof(tmp),"%s%.04f seconds",
				ret[0]?", ":"",
				ts);
		strncat(ret,tmp, 1024 - strlen(ret) - 1);
	}
	return ret;
}