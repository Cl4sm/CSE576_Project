int main(int argc, char *argv[]) {


    struct tm		*Time;
    XEvent		event;
    int			i, n, wid, extrady, extradx;
    int 		Year, Month, DayOfWeek, DayOfMonth, OldDayOfMonth;
    int			Hours, Mins, Secs, OldSecs, digit, xoff, D[10], xsize;
    long		CurrentLocalTime;
    double		UT, TU, TU2, TU3, T0, gmst, jd(), hour24();


    /*
     *  Parse any command line arguments.
     */
    ParseCMDLine(argc, argv);






    /*
     *  Set the font
     */
    if (UseTekton && !ShowSeconds){

	for (i=0; i<11; ++i) xsDigits[i] = xsDigits_Tekton12[i];
	for (i=0; i<11; ++i) xeDigits[i] = xeDigits_Tekton12[i];
	for (i=0; i<11; ++i) xdDigits[i] = xdDigits_Tekton12[i];
	yDigits = yDigits_Tekton12;
	ydDigits = ydDigits_Tekton12;
	extrady = -1;
	extradx = 0;

    } else if (UseTekton && ShowSeconds){

	for (i=0; i<11; ++i) xsDigits[i] = xsDigits_Tekton12n[i];
	for (i=0; i<11; ++i) xeDigits[i] = xeDigits_Tekton12n[i];
	for (i=0; i<11; ++i) xdDigits[i] = xdDigits_Tekton12n[i];
	yDigits = yDigits_Tekton12n;
	ydDigits = ydDigits_Tekton12n;
	extrady = -2;
	extradx = 1;

    } else if (UseLuggerbug && !ShowSeconds){

	for (i=0; i<11; ++i) xsDigits[i] = xsDigits_Luggerbug13[i];
	for (i=0; i<11; ++i) xeDigits[i] = xeDigits_Luggerbug13[i];
	for (i=0; i<11; ++i) xdDigits[i] = xdDigits_Luggerbug13[i];
	yDigits = yDigits_Luggerbug13;
	ydDigits = ydDigits_Luggerbug13;
	extrady = -2;
	extradx = 1;

    } else if (UseLuggerbug && ShowSeconds){

	for (i=0; i<11; ++i) xsDigits[i] = xsDigits_Luggerbug13n[i];
	for (i=0; i<11; ++i) xeDigits[i] = xeDigits_Luggerbug13n[i];
	for (i=0; i<11; ++i) xdDigits[i] = xdDigits_Luggerbug13n[i];
	yDigits = yDigits_Luggerbug13n;
	ydDigits = ydDigits_Luggerbug13n;
	extrady = -2;
	extradx = 1;

    } else if (UseComicSans && !ShowSeconds){

	for (i=0; i<11; ++i) xsDigits[i] = xsDigits_ComicSans11[i];
	for (i=0; i<11; ++i) xeDigits[i] = xeDigits_ComicSans11[i];
	for (i=0; i<11; ++i) xdDigits[i] = xdDigits_ComicSans11[i];
	yDigits = yDigits_ComicSans11;
	ydDigits = ydDigits_ComicSans11;
	extrady = -1;
	extradx = 1;

    } else if (UseComicSans && ShowSeconds){

	for (i=0; i<11; ++i) xsDigits[i] = xsDigits_ComicSans12n[i];
	for (i=0; i<11; ++i) xeDigits[i] = xeDigits_ComicSans12n[i];
	for (i=0; i<11; ++i) xdDigits[i] = xdDigits_ComicSans12n[i];
	yDigits = yDigits_ComicSans12n;
	ydDigits = ydDigits_ComicSans12n;
	extrady = -1;
	extradx = 1;

    } else if (UseJazzPoster && !ShowSeconds){

	for (i=0; i<11; ++i) xsDigits[i] = xsDigits_JazzPoster12[i];
	for (i=0; i<11; ++i) xeDigits[i] = xeDigits_JazzPoster12[i];
	for (i=0; i<11; ++i) xdDigits[i] = xdDigits_JazzPoster12[i];
	yDigits = yDigits_JazzPoster12;
	ydDigits = ydDigits_JazzPoster12;
	extrady = -1;
	extradx = 0;

    } else if (UseJazzPoster && ShowSeconds){

	for (i=0; i<11; ++i) xsDigits[i] = xsDigits_JazzPoster12n[i];
	for (i=0; i<11; ++i) xeDigits[i] = xeDigits_JazzPoster12n[i];
	for (i=0; i<11; ++i) xdDigits[i] = xdDigits_JazzPoster12n[i];
	yDigits = yDigits_JazzPoster12n;
	ydDigits = ydDigits_JazzPoster12n;
	extrady = -1;
	extradx = 1;

    } else {

	for (i=0; i<11; ++i) xsDigits[i] = xsDigits_Arial10[i];
	for (i=0; i<11; ++i) xeDigits[i] = xeDigits_Arial10[i];
	for (i=0; i<11; ++i) xdDigits[i] = xdDigits_Arial10[i];
	yDigits = yDigits_Arial10;
	ydDigits = ydDigits_Arial10;
	extrady = 0;
	extradx = 0;

    }


    /*
     *  Compute widths of digits etc...
     *  Should hand-encode for efficiency, but its easier to do this for development...
     */
    for (i=0; i<12; ++i)  xdMonth[i] = xeMonth[i] - xsMonth[i] + 1;
    for (i=0; i<7;  ++i)  xdDayOfWeek[i] = xeDayOfWeek[i] - xsDayOfWeek[i] + 1;
    for (i=0; i<31; ++i)  xdDayOfMonth[i] = xeDayOfMonth[i] - xsDayOfMonth[i] + 1;
    for (i=0; i<31; ++i)  xdDayOfMonth2[i] = xeDayOfMonth2[i] - xsDayOfMonth[i] + 1;
    for (i=0; i<11; ++i)  xdDigits[i] = xeDigits[i] - xsDigits[i] + 1;
    for (i=0; i<2;  ++i)  xdAMPM[i] = xeAMPM[i] - xsAMPM[i] + 1;



  
   


   
    
    initXwindow(argc, argv);
    if (DisplayDepth <= 8) UseLowColorPixmap = 1;

    if (UseLowColorPixmap)
        openXwindow(argc, argv, wmCalClock_master_LowColor, wmCalClock_mask_bits, wmCalClock_mask_width, wmCalClock_mask_height);
    else
        openXwindow(argc, argv, wmCalClock_master, wmCalClock_mask_bits, wmCalClock_mask_width, wmCalClock_mask_height);





   
    /*
     *  Loop until we die
     */
    n = 32000;
    OldDayOfMonth = -1;
    OldSecs = -1;
    while(1) {


	/*
	 *  Only process every 10th cycle of this loop. We run it faster 
 	 *  to catch expose events, etc...
	 *
	 */
	if ( HasExecute == 0 || n>10){

	    n = 0;

	    if (ShowGreenwichTime){

                CurrentLocalTime = time(CurrentTime);
	        Time = gmtime(&CurrentLocalTime);
	        DayOfMonth = Time->tm_mday-1;
	        DayOfWeek = Time->tm_wday;
	        Month = Time->tm_mon;
	        Hours = Time->tm_hour;
	        Mins  = Time->tm_min;
	        Secs  = Time->tm_sec;

	    } else if (ShowSiderealTime){

		Show24HourTime = 1;
                CurrentLocalTime = time(CurrentTime);
	        Time = gmtime(&CurrentLocalTime);
	        DayOfMonth = Time->tm_mday-1;
	        DayOfWeek = Time->tm_wday;
		Year  = Time->tm_year + 1900; /* this is NOT a Y2K bug */
	        Month = Time->tm_mon;
	        Hours = Time->tm_hour;
	        Mins  = Time->tm_min;
	        Secs  = Time->tm_sec;
		UT = (double)Hours + (double)Mins/60.0 + (double)Secs/3600.0;

		/*
		 *  Compute Greenwich Mean Sidereal Time (gmst)
		 *  The TU here is number of Julian centuries
		 *  since 2000 January 1.5
		 *  From the 1996 astronomical almanac
		 */
		TU = (jd(Year, Month+1, DayOfMonth+1, 0.0) - 2451545.0)/36525.0;
		TU2 = TU*TU;
		TU3 = TU2*TU;
		T0 = (6.0 + 41.0/60.0 + 50.54841/3600.0) + 8640184.812866/3600.0*TU
			+ 0.093104/3600.0*TU2 - 6.2e-6/3600.0*TU3;
		gmst = hour24(hour24(T0) + UT*1.002737909 + Longitude/15.0);
		Hours = (int)gmst;
		gmst  = (gmst - (double)Hours)*60.0;
		Mins  = (int)gmst;
		gmst  = (gmst - (double)Mins)*60.0;
		Secs  = (int)gmst;

	    } else {

                CurrentLocalTime = time(CurrentTime);
	        Time = localtime(&CurrentLocalTime);
	        DayOfMonth = Time->tm_mday-1;
	        DayOfWeek = Time->tm_wday;
	        Month = Time->tm_mon;
	        Hours = Time->tm_hour;
	        Mins  = Time->tm_min;
	        Secs  = Time->tm_sec;

	    }

	    /*
	     *  Flag indicates AM (Flag=0)  or PM (Flag=1)
	     */
	    if (!Show24HourTime){
		Flag  = (Hours >= 12) ? 1 : 0;
		if (Hours == 0)
		    Hours = 12;
		else 
		    Hours = (Hours > 12) ? Hours-12 : Hours;
	    }




	    /*
	     *  Blank the HH:MM section....
	     */
	    xsize = 0;
	    /* dont show leading zeros */
	    if ((digit = Hours / 10) > 0){
	        D[0] = digit, xsize += (xdDigits[digit]+1);
	    } else{
		D[0] = -1;
	    }
	    digit = Hours % 10, D[1] = digit, xsize += (xdDigits[digit]+1);
	    digit = 10,         D[2] = digit, xsize += (xdDigits[digit]+1);
	    digit = Mins / 10,  D[3] = digit, xsize += (xdDigits[digit]+1);
	    digit = Mins % 10,  D[4] = digit, xsize += (xdDigits[digit]+1);
	    if (ShowSeconds){
	        digit = 10,         D[5] = digit, xsize += (xdDigits[digit]+1);
	        digit = Secs / 10,  D[6] = digit, xsize += (xdDigits[digit]+1);
	        digit = Secs % 10,  D[7] = digit, xsize += (xdDigits[digit]);
	    }
	    xoff = ((Hours>9)&&(!Show24HourTime)&&(ShowSeconds)) ? 28 - xsize/2 : 31 - xsize/2;
	    copyXPMArea(5, 110, 54, 15, 5, 5);


            /* 
             *  Draw Hours
             */
	    
	    /* dont show leading zeros */
	    if (D[0] > -1){
	    	digit = D[0];
	    	copyXPMArea(xsDigits[digit], yDigits, xdDigits[digit], ydDigits, xoff+extradx, 7+extrady);
	    	xoff += (xdDigits[digit]+1);
	    }

	    digit = D[1];
	    copyXPMArea(xsDigits[digit], yDigits, xdDigits[digit], ydDigits, xoff+extradx, 7+extrady);
	    xoff += (xdDigits[digit]+1);

            /* 
             *  Draw Colon
             */
	    digit = 10;
	    copyXPMArea(xsDigits[digit], yDigits, xdDigits[digit], ydDigits, xoff+extradx, 7+extrady);
	    xoff += (xdDigits[digit]+1);

            /* 
             *  Draw Minutes
             */
	    digit = D[3];
	    copyXPMArea(xsDigits[digit], yDigits, xdDigits[digit], ydDigits, xoff+extradx, 7+extrady);
	    xoff += (xdDigits[digit]+1);

	    digit = D[4];
	    copyXPMArea(xsDigits[digit], yDigits, xdDigits[digit], ydDigits, xoff+extradx, 7+extrady);
	    xoff += (xdDigits[digit]+1);

	    if (ShowSeconds){

            	/* 
            	 *  Draw Colon
            	 */
	    	digit = 10;
	    	copyXPMArea(xsDigits[digit], yDigits, xdDigits[digit], ydDigits, xoff+extradx, 7+extrady);
	    	xoff += (xdDigits[digit]+1);

            	/* 
            	 *  Draw Seconds
            	 */
	    	digit = D[6];
	    	copyXPMArea(xsDigits[digit], yDigits, xdDigits[digit], ydDigits, xoff+extradx, 7+extrady);
	    	xoff += (xdDigits[digit]+1);

	    	digit = D[7];
	    	copyXPMArea(xsDigits[digit], yDigits, xdDigits[digit], ydDigits, xoff+extradx, 7+extrady);
	    	xoff += (xdDigits[digit]+3);

	    }


	    /*
	     *  Draw AM/PM indicator if we are using 12 Hour Clock.
	     *  Dont show it if we are using 24 Hour Clock.
	     */
	    if (!Show24HourTime)
	        copyXPMArea(xsAMPM[Flag], yAMPM, xdAMPM[Flag], ydAMPM, 54, 5);





	    /*
	     *  Beep on the hour
	     */
	    if (Beep){
		if ((Mins == 0)&&(Secs == 0)&&(OldSecs != Secs)) XBell(display, Volume);
		OldSecs = Secs;
		
	    }






	    if (OldDayOfMonth != DayOfMonth){


	        /*
	         *  Blank the Calendar section....
	         */
	        copyXPMArea(5, 70, 54, 35, 5, 24);


                /* 
                 *  Draw Day of Week and Month
                 */
		wid = xdDayOfWeek[DayOfWeek] + xdMonth[Month] + 1;
	        copyXPMArea(xsDayOfWeek[DayOfWeek], yDayOfWeek, xdDayOfWeek[DayOfWeek], 
						ydMonth, 33-wid/2, 64-24-4-12);
	        copyXPMArea(xsMonth[Month], yMonth, xdMonth[Month], 
					ydMonth, 33-wid/2+xdDayOfWeek[DayOfWeek]+1, 64-24-4-12);



	        /* 
	         *  Draw Day of Month
	         */
	        copyXPMArea(xsDayOfMonth[DayOfMonth], yDayOfMonth[DayOfMonth], xdDayOfMonth[DayOfMonth], ydDayOfMonth, 32-xdDayOfMonth2[DayOfMonth]/2, 36);


	    }


	    OldDayOfMonth = DayOfMonth;




	} else {

	    /*
	     *  Update the counter. 
	     */
	    ++n;

	}





        /*
         *  Double Click Delays
         *  Keep track of click events. If Delay too long, set GotFirstClick's to False.
         */
        if (DblClkDelay > 15) {

            DblClkDelay = 0;
            GotFirstClick1 = 0; GotDoubleClick1 = 0;
            GotFirstClick2 = 0; GotDoubleClick2 = 0;
            GotFirstClick3 = 0; GotDoubleClick3 = 0;

        } else {

            ++DblClkDelay;

        }




	/* 
	 *   Process any pending X events.
	 */
        while(XPending(display)){
            XNextEvent(display, &event);
            switch(event.type){
                case Expose:
                        RedrawWindow();
                        break;
                case ButtonPress:
                        ButtonPressEvent(&event.xbutton);
                        break;
                case ButtonRelease:
                        break;
            }
        }





	
	/* 
	 *  Redraw and wait for next update 
	 */
	RedrawWindow();
	if( HasExecute == 1) {
	  usleep(DELAY);
	} else if( ShowSeconds == 1) {
	  usleep( 200000L);
	} else {
	  usleep( 500000L);
	}


     }



}
