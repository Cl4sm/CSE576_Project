void* sendsequence (void *parameters)
{

	/* YYY check if li,... are long enough if inifinite number will be sent. Maybe put them into double */
        long li2, li, gap = 0, sentnumber = 0, seconds = 0, gap3 = 0, gap4 = 0;
        struct timeval nowstr, nowstr1, first, first1, last, last1;
        int j, c, fd;
        char buff[100];
	struct sockaddr_ll sa;
        struct ifreq ifr;

        struct params* p = (struct params*) parameters;

        /* do we have the rights to do that? */
        if (getuid() && geteuid()) {
                //printf("Sorry but need the su rights!\n");
		gdk_threads_enter ();
                	
			snprintf(buff, 100, "  Problems with sending");
                	gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

                	error("Sorry but need the su rights!");
		gdk_flush();gdk_threads_leave ();
                return NULL;
        }

        /* open socket in raw mode */
        fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        if (fd == -1) {
                //printf("Error: Could not open socket!\n");
		gdk_threads_enter ();

                        snprintf(buff, 100, "  Problems with sending");
                        gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

	                error("Error: Could not open socket!");
		gdk_flush();gdk_threads_leave ();
                return NULL;
        }

	/* which interface would you like to use? */
        memset(&ifr, 0, sizeof(ifr));
        strncpy (ifr.ifr_name, iftext, sizeof(ifr.ifr_name) - 1);
        ifr.ifr_name[sizeof(ifr.ifr_name)-1] = '\0';

        if (ioctl(fd, SIOCGIFINDEX, &ifr) == -1) {
                //printf("No such interface: %s\n", iftext);
		gdk_threads_enter ();

                        snprintf(buff, 100, "  Problems with sending");
                        gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

                	snprintf(buff, 100, "No such interface: %s", iftext);
			error(buff);
		gdk_flush();gdk_threads_leave ();
                close(fd);
                return NULL;
        }

	/* is the interface up? */
        ioctl(fd, SIOCGIFFLAGS, &ifr);
	if ( (ifr.ifr_flags & IFF_UP) == 0) {
                //printf("Interface %s is down\n", iftext);
		gdk_threads_enter ();

                        snprintf(buff, 100, "  Problems with sending");
                        gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

                	snprintf(buff, 100, "Interface %s is down", iftext);
                	error(buff);
		gdk_flush();gdk_threads_leave ();
                close(fd);
                return NULL;
        }

	/* just write in the structure again */
        ioctl(fd, SIOCGIFINDEX, &ifr);

        /* well we need this to work, don't ask me what is it about */
        memset(&sa, 0, sizeof (sa));
        sa.sll_family    = AF_PACKET;
        sa.sll_ifindex   = ifr.ifr_ifindex;
        sa.sll_protocol  = htons(ETH_P_ALL);

        /* this is the time we started */
        gettimeofday(&first, NULL);

        /* to start first sequence immedialtelly */
        gap = p->del;

	gtk_widget_set_sensitive (p->button1, FALSE);
	gtk_widget_set_sensitive (p->button2, FALSE);
	gtk_widget_set_sensitive (p->button3, FALSE);
	gtk_widget_set_sensitive (p->button4, FALSE);
	gtk_widget_set_sensitive (p->button5, FALSE);
	gtk_widget_set_sensitive (p->button6, FALSE);

	/* we check with == -3 if the infinite option was choosed */
	for (li = 0; p->count == -3 ? : li < p->count; li++) {
		/* so wait the delay between sequences */
		while (gap < p->del) {
			gettimeofday(&nowstr, NULL);

			gap = (nowstr.tv_sec*1000000 + nowstr.tv_usec) -
						(last.tv_sec*1000000 + last.tv_usec);

			gap4 = nowstr.tv_sec - first.tv_sec;

			/* if the flag is set - the user clicked the stop button, we quit */
			if (stop_flag == 1) {
				gdk_threads_enter ();

				gtk_widget_set_sensitive (p->button1, TRUE);
				gtk_widget_set_sensitive (p->button2, TRUE);
				gtk_widget_set_sensitive (p->button3, TRUE);
				gtk_widget_set_sensitive (p->button4, TRUE);
				gtk_widget_set_sensitive (p->button5, TRUE);
				gtk_widget_set_sensitive (p->button6, TRUE);

				snprintf(buff, 100, "  Completed %ld cycles.  Sent %ld packets on %s",
										 li,  sentnumber, iftext);
				gtk_statusbar_push(GTK_STATUSBAR(p->button),
								GPOINTER_TO_INT(p->context_id), buff);

				gdk_flush();gdk_threads_leave ();
				close(fd);
				return NULL;
			}

			/* update the status bar every second and display number of sent packets */
			if (gap4 > seconds) {
				gdk_threads_enter ();

				snprintf(buff, 100, "   Cycle: %ld   Starting ...           Sent %ld packets on %s",	li+1, sentnumber, iftext);

				gtk_statusbar_push(GTK_STATUSBAR(p->button), 
							GPOINTER_TO_INT(p->context_id), buff);

				gdk_flush();gdk_threads_leave ();

				seconds++;
			}
		}
		
		/* so we waited the desired time between sequences, now we go through all ten fields
		and send it if there is a name for a packet, and disable button is not on */
		for(j = 0; j < 10; j++) {
			/* skip it if there is no packet name */
			if (p->partable[j][0] == 0)
				continue;
			/* skip it if disable button is activated */
			if (p->partable[j][5] == 0)
				continue;

			/* now we are inside one sequence */
			/* this is the time we started */
			gettimeofday(&first1, NULL);
			gettimeofday(&last1, NULL);
			gettimeofday(&nowstr1, NULL);

			/* to send first packet immedialtelly */
			gap3 = p->partable[j][3];

			/* now we will send this packet partable[j][2] number of times */
			for (li2 = 0; li2 < p->partable[j][2]; li2++) {
				/* wait enough time */
				while (gap3 < p->partable[j][3]) {
					gettimeofday(&nowstr1, NULL);
					/* timing type */
					if (p->timeflag == 1)
						gap3 = (nowstr1.tv_sec*1000000 + nowstr1.tv_usec) -
							(last1.tv_sec*1000000 + last1.tv_usec);
					else
						gap3 = (nowstr1.tv_sec*1000000 + nowstr1.tv_usec) -
							(first1.tv_sec*1000000 + first1.tv_usec) - 
							( (li2 - 1) * p->partable[j][3]);

					gap4 = nowstr1.tv_sec - first.tv_sec;

					/* if the flag is set - the user clicked the stop button, we quit */
					if (stop_flag == 1) {
						gdk_threads_enter ();

						gtk_widget_set_sensitive (p->button1, TRUE);
						gtk_widget_set_sensitive (p->button2, TRUE);
						gtk_widget_set_sensitive (p->button3, TRUE);
						gtk_widget_set_sensitive (p->button4, TRUE);
						gtk_widget_set_sensitive (p->button5, TRUE);
						gtk_widget_set_sensitive (p->button6, TRUE);

						snprintf(buff, 100, "  Stoped in %ld cycle and row %d.  Sent %ld packets on %s", li+1, j+1, sentnumber, iftext);
						gtk_statusbar_push(GTK_STATUSBAR(p->button),
								GPOINTER_TO_INT(p->context_id), buff);

						gdk_flush();gdk_threads_leave ();
						close(fd);
						return NULL;
					}

					/* update the status bar every second and display number 
					 * of sent packets */
					if (gap4 > seconds) {
						gdk_threads_enter ();

						snprintf(buff, 100, "   Cycle: %ld   Row: %d   Sending...   Sent %ld packets on %s", li+1, j+1, sentnumber, iftext);
						gtk_statusbar_push(GTK_STATUSBAR(p->button), 
									GPOINTER_TO_INT(p->context_id), buff);

						gdk_flush();gdk_threads_leave ();

						seconds++;
					}

				}

				/* put the packet on the wire */
				//printf("takle je paket: %s\n", p->pkttable[j]);
				c = sendto(fd, p->pkttable[j], p->partable[j][1], 0, 
								(struct sockaddr *)&sa, sizeof (sa));

				last1.tv_sec = nowstr1.tv_sec;
				last1.tv_usec = nowstr1.tv_usec;
				gap3 = 0;

				if (c > 0)
					sentnumber++;

			}
			
			/* here we gonna wait the desired time before sending the next row */
			gettimeofday(&last1, NULL);
			gap3 = 0;

			while (gap3 < p->partable[j][4]) {

				gettimeofday(&nowstr1, NULL);
				gap3 = (nowstr1.tv_sec*1000000 + nowstr1.tv_usec) -
						(last1.tv_sec*1000000 + last1.tv_usec);
				gap4 = nowstr1.tv_sec - first.tv_sec;

				/* if the flag is set - the user clicked the stop button, we quit */
				if (stop_flag == 1) {
					gdk_threads_enter ();

					gtk_widget_set_sensitive (p->button1, TRUE);
					gtk_widget_set_sensitive (p->button2, TRUE);
					gtk_widget_set_sensitive (p->button3, TRUE);
					gtk_widget_set_sensitive (p->button4, TRUE);
					gtk_widget_set_sensitive (p->button5, TRUE);
					gtk_widget_set_sensitive (p->button6, TRUE);

					snprintf(buff, 100, "  Stoped in %ld cycle and row %d.  Sent %ld packets on %s", li+1, j+1, sentnumber, iftext);
					gtk_statusbar_push(GTK_STATUSBAR(p->button),
									GPOINTER_TO_INT(p->context_id), buff);

					gdk_flush();gdk_threads_leave ();
					close(fd);
					return NULL;
				}

				/* update the status bar every second and display number of sent packets */
				if (gap4 > seconds) {
					gdk_threads_enter ();

					snprintf(buff, 100, "   Cycle: %ld   Row: %d   Waiting...    Sent %ld packets on %s", li+1, j+1, sentnumber, iftext);
					gtk_statusbar_push(GTK_STATUSBAR(p->button), 
								GPOINTER_TO_INT(p->context_id), buff);

					gdk_flush();gdk_threads_leave ();

					seconds++;
				}

			}
		}		
	        gettimeofday(&last, NULL);
                gap = 0;
	}

	/* we sent all the packets, let's activate the toolbar */
	gdk_threads_enter ();

	gtk_widget_set_sensitive (p->button1, TRUE);
	gtk_widget_set_sensitive (p->button2, TRUE);
	gtk_widget_set_sensitive (p->button3, TRUE);
	gtk_widget_set_sensitive (p->button4, TRUE);
	gtk_widget_set_sensitive (p->button5, TRUE);
	gtk_widget_set_sensitive (p->button6, TRUE);

	snprintf(buff, 100, "  Completed %ld cycles. Sent %ld packets on %s", li, sentnumber, iftext);
	gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

	if (close(fd) != 0) {
		//printf("Warning! close(fd) returned -1!\n");
		error("Warning! close(fd) returned -1!");
	}

	gdk_flush();gdk_threads_leave ();

	return NULL;

}