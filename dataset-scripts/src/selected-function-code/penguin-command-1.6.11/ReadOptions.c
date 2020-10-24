void ReadOptions()
{
    FILE *file;
    int i;
        
    file=OpenOptionsFile("r");
    
    if (file==NULL) {
	file=OpenOptionsFile("w");
	if (file==NULL) exit(1);
	
	fprintf(file, "# Penguin Command Options File\n\n");
	fprintf(file, "Sam\nLion\nTux\nThue\nTesmako\nChristian\nAkawaka\nDust Puppy\nMichael\nAndreas\n");
	for (i=0;i<10;i++)
	    fprintf(file, "%d\n", 5000-i*500);
	
	fclose(file);
	file=OpenOptionsFile("r");
    }

    fscanf(file, "%*s%*s%*s%*s%*s\n");
    for (i=0;i<10;i++)
	fscanf(file, "%[^\n]\n",Player[i]);
    for (i=0;i<10;i++)
	fscanf(file, "%d\n",&Hiscore[i]);
    if   ((fscanf(file, "%d\n",&LowDetail)==EOF)
	||(fscanf(file, "%d\n",&MediumDetail)==EOF)
	||(fscanf(file, "%d\n",&HighDetail)==EOF)
	||(fscanf(file, "%d\n",&MusicVol)==EOF)
	||(fscanf(file, "%d\n",&SoundVol)==EOF))
	WriteOptions();
//    for (i=0;i<10;i++)
//	printf("%s %d\n",Player[i],Hiscore[i]);
    
    fclose(file);
}