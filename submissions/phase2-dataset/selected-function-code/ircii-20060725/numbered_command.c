numbered_command(from, comm, ArgList)
	u_char	*from;
	int	comm;
	u_char	**ArgList;
{
	u_char	*user;
	u_char	none_of_these = 0;
	u_char	blah[BIG_BUFFER_SIZE];
	int	flag,
		lastlog_level;
#if 0
	int	user_cnt,
		inv_cnt,
		server_cnt;
#endif /* 0 */

	if (!from || !*from)
		return;
	if (!*ArgList[0])
		user = (u_char *) 0;
	else
		user = ArgList[0];
	if (!ArgList[1])
		return;
	lastlog_level = set_lastlog_msg_level(LOG_CRAP);
	save_message_from();
	message_from((u_char *) 0, LOG_CRAP);
	ArgList++;
	current_numeric = -comm;	/* must be negative of numeric! */
	switch (comm)
	{
	case 001:	/* #define RPL_WELCOME          001 */
		PasteArgs(ArgList, 0);
		if (my_strcmp(user, get_server_nickname(from_server)) != 0)
		{
			yell("=== Setting this servers nickname to \"%s\" from \"%s\"", user, get_server_nickname(from_server));
			set_server_nickname(from_server, user);
		}
		if (do_hook(current_numeric, "%s %s", from, *ArgList)) 
			display_msg(from, ArgList);
		clean_whois_queue();
		break;
	case 002:	/* #define RPL_YOURHOST         002 */
		PasteArgs(ArgList, 0);
		snprintf(CP(blah), sizeof blah, "*** %s", ArgList[0]);
		got_initial_version(blah);
		if (do_hook(current_numeric, "%s %s", from, *ArgList))
			display_msg(from, ArgList);
		break;

/* should do something with this some day, 2.8 had channel/user mode switches */
	case 004:	/* #define RPL_MYINFO           004 */
		PasteArgs(ArgList, 0);
		if (do_hook(current_numeric, "%s %s", from, *ArgList))
			display_msg(from, ArgList);
		break;

/*
 * this part of ircii has been broken for most of ircd 2.7, so someday I'll
 * make it work for ircd 2.8 ...  phone..
 */
#if 0
	case 251:		/* #define RPL_LUSERCLIENT      251 */
		display_msg(from, ArgList);
		if (server_list[from_server].connected)
			break;
		if ((from_server == primary_server) && ((sscanf(ArgList[1],
		    "There are %d users and %d invisible on %d servers",
		    &user_cnt, &inv_cnt, &server_cnt) == 3)||(sscanf(ArgList[1],
		    "There are %d users and %d invisible on %d servers",
		    &user_cnt, &inv_cnt, &server_cnt) == 3)))
		{
			user_cnt =+ inv_cnt;
			if ((server_cnt < get_int_var(MINIMUM_SERVERS_VAR)) ||
			    (user_cnt < get_int_var(MINIMUM_USERS_VAR)))
			{
				say("Trying better populated server...");
				get_connected(from_server + 1);
			}
		}
		break;
#endif /* 0 */
	case 301:		/* #define RPL_AWAY             301 */
		user_is_away(from, ArgList);
		break;

	case 302:		/* #define RPL_USERHOST         302 */
		userhost_returned(from, ArgList);
		break;

	case 303:		/* #define RPL_ISON             303 */
		ison_returned(from, ArgList);
		break;

	case 311:		/* #define RPL_WHOISUSER        311 */
		whois_name(from, ArgList);
		break;

	case 312:		/* #define RPL_WHOISSERVER      312 */
		whois_server(from, ArgList);
		break;

	case 313:		/* #define RPL_WHOISOPERATOR    313 */
		whois_oper(from, ArgList);
		break;

	case 314:		/* #define RPL_WHOWASUSER       314 */
		whowas_name(from, ArgList);
		break;

	case 316:		/* #define RPL_WHOISCHANOP      316 */
		whois_chop(from, ArgList);
		break;

	case 317:		/* #define RPL_WHOISIDLE        317 */
		whois_lastcom(from, ArgList);
		break;

	case 318:		/* #define RPL_ENDOFWHOIS       318 */
		end_of_whois(from, ArgList);
		break;

	case 319:		/* #define RPL_WHOISCHANNELS    319 */
		whois_channels(from, ArgList);
		break;

	case 321:		/* #define RPL_LISTSTART        321 */
		ArgList[0] = UP("Channel\0Users\0Topic");
		ArgList[1] = ArgList[0] + 8;
		ArgList[2] = ArgList[1] + 6;
		ArgList[3] = (u_char *) 0;
		funny_list(from, ArgList);
		break;

	case 322:		/* #define RPL_LIST             322 */
		funny_list(from, ArgList);
		break;

	case 324:		/* #define RPL_CHANNELMODEIS    324 */
		funny_mode(from, ArgList);
		break;

	case 341:		/* #define RPL_INVITING         341 */
		invite(from, ArgList);
		break;

	case 352:		/* #define RPL_WHOREPLY         352 */
		whoreply((u_char *) 0, ArgList);
		break;

	case 353:		/* #define RPL_NAMREPLY         353 */
		funny_namreply(from, ArgList);
		break;

	case 366:		/* #define RPL_ENDOFNAMES       366 */
		{
			u_char	*tmp = (u_char *) 0,
				*chan;
			ChannelList	*ptr = (ChannelList *) 0;

			PasteArgs(ArgList, 0);
			malloc_strcpy(&tmp, ArgList[0]);
			chan = next_arg(tmp, 0);
			flag = do_hook(current_numeric, "%s %s", from, ArgList[0]);
			
			if ((ptr = lookup_channel(chan, from_server, CHAN_NOUNLINK)) && !((ptr->status & CHAN_NAMES) && (ptr->status & CHAN_MODE)))
				if (get_int_var(SHOW_END_OF_MSGS_VAR) && flag)
					display_msg(from, ArgList);

			new_free(&tmp);
		}
		break;

	case 381: 		/* #define RPL_YOUREOPER        381 */
		PasteArgs(ArgList, 0);
		if (do_hook(current_numeric, "%s %s", from, *ArgList))
			display_msg(from, ArgList);
		set_server_operator(parsing_server_index, 1);
		update_all_status();	/* fix the status line */
		break;

	case 401:		/* #define ERR_NOSUCHNICK       401 */
		no_such_nickname(from, ArgList);
		break;

	case 405:		/* #define ERR_TOOMANYCHANNELS  405 */
		remove_channel(ArgList[0], parsing_server_index);
		break;
		
	case 421:		/* #define ERR_UNKNOWNCOMMAND   421 */
		if (check_screen_redirect(ArgList[0]))
			break;
		if (check_wait_command(ArgList[0]))
			break;
		PasteArgs(ArgList, 0);
		flag = do_hook(current_numeric, "%s %s", from, *ArgList);
		if (!my_strncmp("ISON", *ArgList, 4) || !my_strncmp("USERHOST",
		    *ArgList, 8))
		{
			set_server_2_6_2(parsing_server_index, 0);
			convert_to_whois();
		}
		else if (flag)
			display_msg(from, ArgList);
		break;

	case 432:		/* #define ERR_ERRONEUSNICKNAME 432 */
	case 433:		/* #define ERR_NICKNAMEINUSE    433 */ 
		PasteArgs(ArgList, 0);
		if (do_hook(current_numeric, "%s %s", from, *ArgList))
			display_msg(from, ArgList);
		reset_nickname(from);
		break;

	case 437:		/* #define ERR_UNAVAILRESOURCE  437 */
		PasteArgs(ArgList, 0);
		if (do_hook(current_numeric, "%s %s", from, *ArgList))
			display_msg(from, ArgList);
		if (!is_channel(*ArgList))
			reset_nickname(from);
		break;

	case 463:		/* #define ERR_NOPERMFORHOST    463 */
		display_msg(from, ArgList);
		close_server(parsing_server_index, empty_string);
		window_check_servers();
		if (!connected_to_server)
			get_connected(parsing_server_index + 1);
		break;

	case 464:		/* #define ERR_PASSWDMISMATCH   464 */
		PasteArgs(ArgList, 0);
		flag = do_hook(current_numeric, "%s %s", from, ArgList[0]);
		if (oper_command)
		{
			if (flag)
				display_msg(from, ArgList);
		}
		else
			get_password();
		break;

	case 465:		/* #define ERR_YOUREBANNEDCREEP 465 */
		{
			int	klined_server = parsing_server_index;

			PasteArgs(ArgList, 0);
			if (do_hook(current_numeric, "%s %s", from, ArgList[0]))
				display_msg(from, ArgList);
			close_server(parsing_server_index, empty_string);
			window_check_servers();
			if (number_of_servers > 1)
			{
				remove_from_server_list(klined_server);
			}
			if (!connected_to_server)
				say("You are not connected to a server. Use /SERVER to connect.");
			break;
		}

	case 471:		/* #define ERR_CHANNELISFULL    471 */
	case 473:		/* #define ERR_INVITEONLYCHAN   473 */
	case 474:		/* #define ERR_BANNEDFROMCHAN   474 */
	case 475: 		/* #define ERR_BADCHANNELKEY    475 */
	case 476:		/* #define ERR_BADCHANMASK      476 */
		cannot_join_channel(from, ArgList);
		break;

	case 484:		/* #define ERR_RESTRICTED       484 */
		if (do_hook(current_numeric, "%s %s", from, *ArgList))
			display_msg(from, ArgList);
		set_server_flag(parsing_server_index, USER_MODE_R, 1);
		break;

		/*
		 * The following accumulates the remaining arguments
		 * in ArgSpace for hook detection.  We can't use
		 * PasteArgs here because we still need the arguments
		 * separated for use elsewhere.
		 */
	default:
		{
			u_char	*ArgSpace = (u_char *) 0;
			int	i,
				do_message_from = 0;
			size_t	len;

			for (i = len = 0; ArgList[i]; len += my_strlen(ArgList[i++]))
				;
			len += (i - 1);
			ArgSpace = new_malloc(len + 1);
			ArgSpace[0] = '\0';
			/* this is cheating */
			if (ArgList[0] && is_channel(ArgList[0]))
				do_message_from = 1;
			for (i = 0; ArgList[i]; i++)
			{
				if (i)
					my_strcat(ArgSpace, " ");
				my_strcat(ArgSpace, ArgList[i]);
			}
			if (do_message_from)
				message_from(ArgList[0], LOG_CRAP);
			i = do_hook(current_numeric, "%s %s", from, ArgSpace);
			new_free(&ArgSpace);
			if (do_message_from)
				restore_message_from();
			if (i == 0)
				goto done;
			none_of_these = 1;
		}
	}
	/* the following do not hurt the ircII if intercepted by a hook */
	if (none_of_these)
	{
		switch (comm)
		{
		case 221: 		/* #define RPL_UMODEIS          221 */
			put_it("%s Your user mode is \"%s\"", numeric_banner(),
				ArgList[0]);
			break;

		case 242:		/* #define RPL_STATSUPTIME      242 */
			PasteArgs(ArgList, 0);
			if (from && !my_strnicmp(get_server_itsname(parsing_server_index),
			    from, my_strlen(get_server_itsname(parsing_server_index))))
				from = NULL;
			if (from)
				put_it("%s %s from (%s)", numeric_banner(),
					*ArgList, from);
			else
				put_it("%s %s", numeric_banner(), *ArgList);
			break;

		case 332:		/* #define RPL_TOPIC            332 */
			channel_topic(from, ArgList);
			break;

		case 351:		/* #define RPL_VERSION          351 */
			version(from, ArgList);
			break;

		case 364:		/* #define RPL_LINKS            364 */
			if (ArgList[2])
			{
				PasteArgs(ArgList, 2);
				put_it("%s %-20s %-20s %s", numeric_banner(),
					ArgList[0], ArgList[1], ArgList[2]);
			}
			else
			{
				PasteArgs(ArgList, 1);
				put_it("%s %-20s %s", numeric_banner(),
					ArgList[0], ArgList[1]);
			}
			break;

		case 372:		/* #define RPL_MOTD             372 */
			if (!get_int_var(SUPPRESS_SERVER_MOTD_VAR) ||
			    !get_server_motd(parsing_server_index))
			{
				PasteArgs(ArgList, 0);
				put_it("%s %s", numeric_banner(), ArgList[0]);
			}
			break;

		case 375:		/* #define RPL_MOTDSTART        375 */
			if (!get_int_var(SUPPRESS_SERVER_MOTD_VAR) ||
			    !get_server_motd(parsing_server_index))
			{
				PasteArgs(ArgList, 0);
				put_it("%s %s", numeric_banner(), ArgList[0]);
			}
			break;

		case 376:		/* #define RPL_ENDOFMOTD        376 */
			if (server_list[parsing_server_index].attempting_to_connect)
				got_initial_version(UP("*** Your host is broken and not running any version"));
			if (get_int_var(SHOW_END_OF_MSGS_VAR) &&
			    (!get_int_var(SUPPRESS_SERVER_MOTD_VAR) ||
			    !get_server_motd(parsing_server_index)))
			{
				PasteArgs(ArgList, 0);
				put_it("%s %s", numeric_banner(), ArgList[0]);
			}
			set_server_motd(parsing_server_index, 0);
			break;

		case 384:		/* #define RPL_MYPORTIS         384 */
			PasteArgs(ArgList, 0);
			put_it("%s %s %s", numeric_banner(), ArgList[0], user);
			break;

		case 385:		/* #define RPL_NOTOPERANYMORE   385 */
			set_server_operator(parsing_server_index, 0);
			display_msg(from, ArgList);
			update_all_status();
			break;

		case 403:		/* #define ERR_NOSUCHCHANNEL    403 */
			not_valid_channel(from, ArgList);
			break;

		case 451:		/* #define ERR_NOTREGISTERED    451 */
	/*
	 * Sometimes the server doesn't catch the USER line, so
	 * here we send a simplified version again  -lynx 
	 */
			send_to_server("USER %s %s . :%s", username,
				(send_umode && *send_umode) ? send_umode : (u_char *) ".",
				realname);
			send_to_server("NICK %s",
				get_server_nickname(parsing_server_index));
			break;

		case 462:		/* #define ERR_ALREADYREGISTRED 462 */
			display_msg(from, ArgList);
			break;

#define RPL_CLOSEEND         363
#define RPL_SERVLISTEND      235
		case 315:		/* #define RPL_ENDOFWHO         315 */
			if (cannot_open != (u_char *) 0)
				yell("Cannot open: %s", cannot_open);

		case 323:               /* #define RPL_LISTEND          323 */
			funny_print_widelist();

		case 219:		/* #define RPL_ENDOFSTATS       219 */
		case 232:		/* #define RPL_ENDOFSERVICES    232 */
		case 365:		/* #define RPL_ENDOFLINKS       365 */
		case 368:		/* #define RPL_ENDOFBANLIST     368 */
		case 369:		/* #define RPL_ENDOFWHOWAS      369 */
		case 374:		/* #define RPL_ENDOFINFO        374 */
#if 0	/* this case needs special handing - see above */
		case 376:		/* #define RPL_ENDOFMOTD        376 */
#endif /* 0 */
		case 394:		/* #define RPL_ENDOFUSERS       394 */
			if (!get_int_var(SHOW_END_OF_MSGS_VAR))
				break;
		default:
			display_msg(from, ArgList);
		}
	}
	set_lastlog_msg_level(lastlog_level);
done:
	restore_message_from();
}
