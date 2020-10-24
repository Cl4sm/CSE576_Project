void WriteOptions()
{
    FILE *file;
    int i;

    file=OpenOptionsFile("w");
    if (file==NULL) exit(1);
    
    fprintf(file, "# Penguin Command Options File\n\n");
    for (i=0;i<10;i++)
	fprintf(file, "%s\n", Player[i]);
    for (i=0;i<10;i++)
	fprintf(file, "%d\n", Hiscore[i]);
    fprintf(file, "%d\n",LowDetail);
    fprintf(file, "%d\n",MediumDetail);
    fprintf(file, "%d\n",HighDetail);
    fprintf(file, "%d\n",MusicVol);
    fprintf(file, "%d\n",SoundVol);

    fclose(file);
}