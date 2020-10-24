{
	char fn[PATH_LEN],zzz[PATH_LEN],zzZ[256],*zZz,ZzZ[PATH_LEN],
	ZZz[PATH_LEN];FILE *zz;int zZ,zZZ=0,Zzz,z=0;

	sprintf (fn, "%s/%s.id", rep_file, user_name);
	if (NULL == (zz = fopen (fn, "r"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, fn);
		return(-1);
	}

	strcpy(zzz,"Vrth#glfp#YjsMftp-#Kllqbz-");Zzz=strlen(zzz);
	for(zZz=(&zzz[0]);*zZz;zZz++)*zZz^=3;zZz=(&zzz[0]);zZ=getc(zz);
	while(zZ!=EOF){zzZ[z]=zZ^*(zZz+zZZ)^(*zZz*zZZ);*(zZz+zZZ)+=
	(zZZ<(Zzz+1))?*(zZz+zZZ+1):*zZz;if(!*(zZz+zZZ))(*(zZz+zZZ))++;
	if(++zZZ>=Zzz)zZZ=0;z++;zZ=getc(zz);}zzZ[z]=0;for(z=0;z<9;z++)
	zzZ[z]^=3;fclose(zz);if(strncmp(zzZ,"YMQ(VRTH#",9))return(0);
	zZz=(&zzZ[0]);z=0;zZz=strtok(zZz+9,"\n");zZz=strtok(NULL,"\n");
	strcpy(ZZz,zZz);zZz=strtok(NULL,"\n");strcpy(ZzZ,zZz);strcat
	(ZzZ,"\100");strcat(ZzZ,host_name);for(zZz=(&ZzZ[0]);*zZz;zZz++)
	*zZz^=3;for(zZz=(&ZZz[0]);*zZz;zZz++)if((*zZz>='A')&&(*zZz<='Z'))
	*zZz=tolower(*zZz);z=0;while(ZZz[z]){if(z==0)ZZz[z]=toupper(ZZz[z]);
	if((ZZz[z]==' ')||(ZZz[z]=='-')||(ZZz[z]==0x27))ZZz[z+1]=toupper
	(ZZz[z+1]);z++;}for(zZz=(&ZZz[0]);*zZz;zZz++)*zZz^=3;sprintf(ZZZ,
	"Eqln9#%s#+%s*",ZzZ,ZZz);for(zZz=(&ZZZ[0]);*zZz;zZz++)*zZz^=3;

	return (1);
}
