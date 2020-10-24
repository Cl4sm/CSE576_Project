void InitSound()
{
#ifdef USE_SOUND
    int i,ChannelNum;
    char text[200];
	
    if (!sound) return;
    puts("** Opening Audio Mixer **");
    if ( Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
        fprintf(stderr,"Warning: Couldn't set 44100 Hz audio\n- Reason: %s\n",SDL_GetError());
        fprintf(stderr,"\t**\nSOUND TURNED OFF\n\t**\n");
        sound=0;
    } else {
		ChannelNum = Mix_AllocateChannels(9)-1;
		printf("   %d channels allocated\n",ChannelNum);
		if (ChannelNum > 2)
			Mix_ReserveChannels(1);
		printf("** Loading Music **\n");
		for (i=0;i<MUSICNUM;i++) {
			sprintf(text,"%s/sound/%s",DATAPATH,ModName[i]);
			Music[i] = Mix_LoadMUS(text);
			if (Music==NULL) printf("COULD NOT LOAD MUSIC: %s\n", SDL_GetError());
		}
		printf("** Loading Sounds **\n");
		sprintf(text,"%s/sound/%s",DATAPATH,"missex.wav");
		MissileExplosionSound = Mix_LoadWAV(text);
		if (MissileExplosionSound == NULL) {
			puts("Sound effects could not be loaded. FATAL ERROR!");
			exit(1);
		}
		sprintf(text,"%s/sound/%s",DATAPATH,"Wooeep.wav");
		CitySound = Mix_LoadWAV(text);
		sprintf(text,"%s/sound/%s",DATAPATH,"Machine.wav");
		GameoverSound = Mix_LoadWAV(text);
		sprintf(text,"%s/sound/%s",DATAPATH,"bombex.wav");
		BombExplosionSound = Mix_LoadWAV(text);
		sprintf(text,"%s/sound/%s",DATAPATH,"click.wav");
		ClickSound = Mix_LoadWAV(text);
		Mix_HookMusicFinished(PlayMusic);
    }
    CurrentMusic=abrand(0,MUSICNUM-1);
#endif
}