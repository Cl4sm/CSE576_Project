void FinalScore()
{
    int i;

    if (Score>Hiscore[9]) {

    char Name[200]="",text[200];
    int a;

    Blit(0,0,BackPic);
    PutString(Screen,400-SFont_TextWidth("Final Score")/2,200,"Final Score");
    sprintf(text,"%d", Score);
    PutString(Screen,400-SFont_TextWidth(text)/2,250,text);
    PutString(Screen,400-SFont_TextWidth("Enter Your Name:")/2,300,"Enter Your Name:");
    Update();
    SFont_Input(Screen, 260, 350, 280, Name);
    if (strcmp(Name,"")==0) strcpy(Name,"Anonym. Coward");
    for (i=0;i<10;i++) {
	if (Score>Hiscore[i]) {
	    // Move other players down on the list
	    for (a=9;a>=i;a--) {
		strcpy(Player[a],Player[a-1]);
		Hiscore[a]=Hiscore[a-1];		
	    }
	    // add the player
	    strcpy(Player[i],Name);
	    Hiscore[i]=Score;
	    break;
	}
    }
//    for (i=0;i<10;i++)
//	printf("%s %d\n",Player[i],Hiscore[i]);
    
    WriteOptions();
}}