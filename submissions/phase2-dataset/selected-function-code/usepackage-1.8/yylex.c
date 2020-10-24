	{
	register yy_state_type yy_current_state;
	register char *yy_cp, *yy_bp;
	register int yy_act;

#line 51 "scanner.l"


#line 617 "scanner.c"

	if ( yy_init )
		{
		yy_init = 0;

#ifdef YY_USER_INIT
		YY_USER_INIT;
#endif

		if ( ! yy_start )
			yy_start = 1;	/* first start state */

		if ( ! yyin )
			yyin = stdin;

		if ( ! yyout )
			yyout = stdout;

		if ( ! yy_current_buffer )
			yy_current_buffer =
				yy_create_buffer( yyin, YY_BUF_SIZE );

		yy_load_buffer_state();
		}

	while ( 1 )		/* loops until end-of-file is reached */
		{
		yy_cp = yy_c_buf_p;

		/* Support of yytext. */
		*yy_cp = yy_hold_char;

		/* yy_bp points to the position in yy_ch_buf of the start of
		 * the current run.
		 */
		yy_bp = yy_cp;

		yy_current_state = yy_start;
yy_match:
		do
			{
			register YY_CHAR yy_c = yy_ec[YY_SC_TO_UI(*yy_cp)];
			if ( yy_accept[yy_current_state] )
				{
				yy_last_accepting_state = yy_current_state;
				yy_last_accepting_cpos = yy_cp;
				}
			while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
				{
				yy_current_state = (int) yy_def[yy_current_state];
				if ( yy_current_state >= 64 )
					yy_c = yy_meta[(unsigned int) yy_c];
				}
			yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
			++yy_cp;
			}
		while ( yy_base[yy_current_state] != 129 );

yy_find_action:
		yy_act = yy_accept[yy_current_state];
		if ( yy_act == 0 )
			{ /* have to back up */
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			yy_act = yy_accept[yy_current_state];
			}

		YY_DO_BEFORE_ACTION;


do_action:	/* This label is used only to access EOF actions. */


		switch ( yy_act )
	{ /* beginning of action switch */
			case 0: /* must back up */
			/* undo the effects of YY_DO_BEFORE_ACTION */
			*yy_cp = yy_hold_char;
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			goto yy_find_action;

case 1:
YY_RULE_SETUP
#line 53 "scanner.l"
/* ignore */
	YY_BREAK
case 2:
YY_RULE_SETUP
#line 54 "scanner.l"
return(COLON);
	YY_BREAK
case 3:
YY_RULE_SETUP
#line 55 "scanner.l"
return(SEMICOLON);
	YY_BREAK
case 4:
YY_RULE_SETUP
#line 56 "scanner.l"
return(COMMA);
	YY_BREAK
case 5:
YY_RULE_SETUP
#line 57 "scanner.l"
return(LEFTPAREN);
	YY_BREAK
case 6:
YY_RULE_SETUP
#line 58 "scanner.l"
return(RIGHTPAREN);
	YY_BREAK
case 7:
YY_RULE_SETUP
#line 59 "scanner.l"
return(WILDCARD);
	YY_BREAK
case 8:
YY_RULE_SETUP
#line 60 "scanner.l"
return(EQUALS);
	YY_BREAK
case 9:
YY_RULE_SETUP
#line 61 "scanner.l"
return(PLUSEQUALS);
	YY_BREAK
case 10:
YY_RULE_SETUP
#line 62 "scanner.l"
return(QEQUALS);
	YY_BREAK
case 11:
YY_RULE_SETUP
#line 63 "scanner.l"
return(QPLUSEQUALS);
	YY_BREAK
case 12:
YY_RULE_SETUP
#line 64 "scanner.l"
return(ASSIGN);
	YY_BREAK
case 13:
YY_RULE_SETUP
#line 65 "scanner.l"
return(WITH);
	YY_BREAK
case 14:
YY_RULE_SETUP
#line 66 "scanner.l"
return(BEGIN_ANNOTATE);
	YY_BREAK
case 15:
YY_RULE_SETUP
#line 67 "scanner.l"
return(END_ANNOTATE);
	YY_BREAK
case 16:
YY_RULE_SETUP
#line 69 "scanner.l"
bufp=litbuf; bufp[0] = '\0'; BEGIN(lit);
	YY_BREAK
case 17:
YY_RULE_SETUP
#line 70 "scanner.l"
strcpy(bufp, yytext); bufp += yyleng;
	YY_BREAK
case 18:
YY_RULE_SETUP
#line 71 "scanner.l"
{ strcpy(bufp, yytext); bufp += yyleng;
		  line_number[stack_pointer]++; }
	YY_BREAK
case 19:
YY_RULE_SETUP
#line 73 "scanner.l"
BEGIN(INITIAL); return(LITERAL);
	YY_BREAK
case 20:
YY_RULE_SETUP
#line 75 "scanner.l"
bufp=litbuf; bufp[0] = '\0'; BEGIN(scpt);
	YY_BREAK
case 21:
YY_RULE_SETUP
#line 76 "scanner.l"
strcpy(bufp, yytext); bufp += yyleng;
	YY_BREAK
case 22:
YY_RULE_SETUP
#line 77 "scanner.l"
{ strcpy(bufp, yytext); bufp += yyleng;
		  line_number[stack_pointer]++; }
	YY_BREAK
case 23:
YY_RULE_SETUP
#line 79 "scanner.l"
BEGIN(INITIAL); return(SCRIPT);
	YY_BREAK
case 24:
YY_RULE_SETUP
#line 81 "scanner.l"
strcpy(litbuf, yytext); return(PATH);
	YY_BREAK
case 25:
YY_RULE_SETUP
#line 82 "scanner.l"
strcpy(litbuf, yytext); return(NAME);
	YY_BREAK
case 26:
YY_RULE_SETUP
#line 83 "scanner.l"
{ strcpy(litbuf, yytext); litbuf[yyleng-1] = '\0';
		  return(PREFIX); }
	YY_BREAK
case 27:
YY_RULE_SETUP
#line 85 "scanner.l"
/* ignore */
	YY_BREAK
case 28:
YY_RULE_SETUP
#line 86 "scanner.l"
line_number[stack_pointer]++;
	YY_BREAK
case 29:
YY_RULE_SETUP
#line 88 "scanner.l"
yytext[yyleng-1] = '\0'; include(yytext+9);
	YY_BREAK
case 30:
YY_RULE_SETUP
#line 90 "scanner.l"
fprintf(stderr, "usepackage: ignoring character `%s' on line %d of %s\n", yytext, line_number[stack_pointer], file_name[stack_pointer]);
	YY_BREAK
case 31:
YY_RULE_SETUP
#line 92 "scanner.l"
ECHO;
	YY_BREAK
#line 858 "scanner.c"
case YY_STATE_EOF(INITIAL):
case YY_STATE_EOF(lit):
case YY_STATE_EOF(scpt):
	yyterminate();

	case YY_END_OF_BUFFER:
		{
		/* Amount of text matched not including the EOB char. */
		int yy_amount_of_matched_text = (int) (yy_cp - yytext_ptr) - 1;

		/* Undo the effects of YY_DO_BEFORE_ACTION. */
		*yy_cp = yy_hold_char;
		YY_RESTORE_YY_MORE_OFFSET

		if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_NEW )
			{
			/* We're scanning a new file or input source.  It's
			 * possible that this happened because the user
			 * just pointed yyin at a new source and called
			 * yylex().  If so, then we have to assure
			 * consistency between yy_current_buffer and our
			 * globals.  Here is the right place to do so, because
			 * this is the first action (other than possibly a
			 * back-up) that will match for the new input source.
			 */
			yy_n_chars = yy_current_buffer->yy_n_chars;
			yy_current_buffer->yy_input_file = yyin;
			yy_current_buffer->yy_buffer_status = YY_BUFFER_NORMAL;
			}

		/* Note that here we test for yy_c_buf_p "<=" to the position
		 * of the first EOB in the buffer, since yy_c_buf_p will
		 * already have been incremented past the NUL character
		 * (since all states make transitions on EOB to the
		 * end-of-buffer state).  Contrast this with the test
		 * in input().
		 */
		if ( yy_c_buf_p <= &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			{ /* This was really a NUL. */
			yy_state_type yy_next_state;

			yy_c_buf_p = yytext_ptr + yy_amount_of_matched_text;

			yy_current_state = yy_get_previous_state();

			/* Okay, we're now positioned to make the NUL
			 * transition.  We couldn't have
			 * yy_get_previous_state() go ahead and do it
			 * for us because it doesn't know how to deal
			 * with the possibility of jamming (and we don't
			 * want to build jamming into it because then it
			 * will run more slowly).
			 */

			yy_next_state = yy_try_NUL_trans( yy_current_state );

			yy_bp = yytext_ptr + YY_MORE_ADJ;

			if ( yy_next_state )
				{
				/* Consume the NUL. */
				yy_cp = ++yy_c_buf_p;
				yy_current_state = yy_next_state;
				goto yy_match;
				}

			else
				{
				yy_cp = yy_c_buf_p;
				goto yy_find_action;
				}
			}

		else switch ( yy_get_next_buffer() )
			{
			case EOB_ACT_END_OF_FILE:
				{
				yy_did_buffer_switch_on_eof = 0;

				if ( yywrap() )
					{
					/* Note: because we've taken care in
					 * yy_get_next_buffer() to have set up
					 * yytext, we can now set up
					 * yy_c_buf_p so that if some total
					 * hoser (like flex itself) wants to
					 * call the scanner after we return the
					 * YY_NULL, it'll still work - another
					 * YY_NULL will get returned.
					 */
					yy_c_buf_p = yytext_ptr + YY_MORE_ADJ;

					yy_act = YY_STATE_EOF(YY_START);
					goto do_action;
					}

				else
					{
					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
					}
				break;
				}

			case EOB_ACT_CONTINUE_SCAN:
				yy_c_buf_p =
					yytext_ptr + yy_amount_of_matched_text;

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_match;

			case EOB_ACT_LAST_MATCH:
				yy_c_buf_p =
				&yy_current_buffer->yy_ch_buf[yy_n_chars];

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_find_action;
			}
		break;
		}

	default:
		YY_FATAL_ERROR(
			"fatal flex scanner internal error--no action found" );
	} /* end of action switch */
		} /* end of scanning one token */
	} /* end of yylex */
