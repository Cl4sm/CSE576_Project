YY_DECL
{
	register yy_state_type yy_current_state;
	register char *yy_cp, *yy_bp;
	register int yy_act;
    
#line 205 "osm_qos_parser_l.l"



#line 1135 "osm_qos_parser_l.c"

	if ( !(yy_init) )
		{
		(yy_init) = 1;

#ifdef YY_USER_INIT
		YY_USER_INIT;
#endif

		if ( ! (yy_start) )
			(yy_start) = 1;	/* first start state */

		if ( ! yyin )
			yyin = stdin;

		if ( ! yyout )
			yyout = stdout;

		if ( ! YY_CURRENT_BUFFER ) {
			yyensure_buffer_stack ();
			YY_CURRENT_BUFFER_LVALUE =
				yy_create_buffer(yyin,YY_BUF_SIZE );
		}

		yy_load_buffer_state( );
		}

	while ( 1 )		/* loops until end-of-file is reached */
		{
		yy_cp = (yy_c_buf_p);

		/* Support of yytext. */
		*yy_cp = (yy_hold_char);

		/* yy_bp points to the position in yy_ch_buf of the start of
		 * the current run.
		 */
		yy_bp = yy_cp;

		yy_current_state = (yy_start);
yy_match:
		do
			{
			register YY_CHAR yy_c = yy_ec[YY_SC_TO_UI(*yy_cp)];
			if ( yy_accept[yy_current_state] )
				{
				(yy_last_accepting_state) = yy_current_state;
				(yy_last_accepting_cpos) = yy_cp;
				}
			while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
				{
				yy_current_state = (int) yy_def[yy_current_state];
				if ( yy_current_state >= 582 )
					yy_c = yy_meta[(unsigned int) yy_c];
				}
			yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
			++yy_cp;
			}
		while ( yy_base[yy_current_state] != 791 );

yy_find_action:
		yy_act = yy_accept[yy_current_state];
		if ( yy_act == 0 )
			{ /* have to back up */
			yy_cp = (yy_last_accepting_cpos);
			yy_current_state = (yy_last_accepting_state);
			yy_act = yy_accept[yy_current_state];
			}

		YY_DO_BEFORE_ACTION;

do_action:	/* This label is used only to access EOF actions. */

		switch ( yy_act )
	{ /* beginning of action switch */
			case 0: /* must back up */
			/* undo the effects of YY_DO_BEFORE_ACTION */
			*yy_cp = (yy_hold_char);
			yy_cp = (yy_last_accepting_cpos);
			yy_current_state = (yy_last_accepting_state);
			goto yy_find_action;

case 1:
/* rule 1 can match eol */
YY_RULE_SETUP
#line 208 "osm_qos_parser_l.l"
{ SAVE_POS; RESET_NEW_LINE_FLAGS; } /* swallow comment */
	YY_BREAK
case 2:
/* rule 2 can match eol */
YY_RULE_SETUP
#line 209 "osm_qos_parser_l.l"
{ SAVE_POS; RESET_NEW_LINE_FLAGS; } /* trailing blanks with new line */
	YY_BREAK
case 3:
YY_RULE_SETUP
#line 210 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; }
	YY_BREAK
case 4:
/* rule 4 can match eol */
YY_RULE_SETUP
#line 211 "osm_qos_parser_l.l"
{ SAVE_POS; RESET_NEW_LINE_FLAGS; }
	YY_BREAK
case 5:
YY_RULE_SETUP
#line 213 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_ULPS_START; }
	YY_BREAK
case 6:
YY_RULE_SETUP
#line 214 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_ULPS_END; }
	YY_BREAK
case 7:
YY_RULE_SETUP
#line 216 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_PORT_GROUPS_START; }
	YY_BREAK
case 8:
YY_RULE_SETUP
#line 217 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_PORT_GROUPS_END; }
	YY_BREAK
case 9:
YY_RULE_SETUP
#line 218 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_PORT_GROUP_START; }
	YY_BREAK
case 10:
YY_RULE_SETUP
#line 219 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_PORT_GROUP_END; }
	YY_BREAK
case 11:
YY_RULE_SETUP
#line 221 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_SETUP_START; }
	YY_BREAK
case 12:
YY_RULE_SETUP
#line 222 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_SETUP_END; }
	YY_BREAK
case 13:
YY_RULE_SETUP
#line 223 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_VLARB_TABLES_START; }
	YY_BREAK
case 14:
YY_RULE_SETUP
#line 224 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_VLARB_TABLES_END; }
	YY_BREAK
case 15:
YY_RULE_SETUP
#line 225 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_VLARB_SCOPE_START; }
	YY_BREAK
case 16:
YY_RULE_SETUP
#line 226 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_VLARB_SCOPE_END; }
	YY_BREAK
case 17:
YY_RULE_SETUP
#line 228 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_SL2VL_TABLES_START; }
	YY_BREAK
case 18:
YY_RULE_SETUP
#line 229 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_SL2VL_TABLES_END; }
	YY_BREAK
case 19:
YY_RULE_SETUP
#line 230 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_SL2VL_SCOPE_START; }
	YY_BREAK
case 20:
YY_RULE_SETUP
#line 231 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_SL2VL_SCOPE_END; }
	YY_BREAK
case 21:
YY_RULE_SETUP
#line 233 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_LEVELS_START; }
	YY_BREAK
case 22:
YY_RULE_SETUP
#line 234 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_LEVELS_END; }
	YY_BREAK
case 23:
YY_RULE_SETUP
#line 235 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_LEVEL_START; }
	YY_BREAK
case 24:
YY_RULE_SETUP
#line 236 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_LEVEL_END; }
	YY_BREAK
case 25:
YY_RULE_SETUP
#line 238 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_MATCH_RULES_START; }
	YY_BREAK
case 26:
YY_RULE_SETUP
#line 239 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_MATCH_RULES_END; }
	YY_BREAK
case 27:
YY_RULE_SETUP
#line 240 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_MATCH_RULE_START; }
	YY_BREAK
case 28:
YY_RULE_SETUP
#line 241 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; return TK_QOS_MATCH_RULE_END; }
	YY_BREAK
case 29:
YY_RULE_SETUP
#line 243 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_PORT_GUID;    return TK_PORT_GUID;  }
	YY_BREAK
case 30:
YY_RULE_SETUP
#line 244 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_PORT_NAME;    return TK_PORT_NAME;  }
	YY_BREAK
case 31:
YY_RULE_SETUP
#line 245 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_PARTITION;    return TK_PARTITION;  }
	YY_BREAK
case 32:
YY_RULE_SETUP
#line 246 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_NODE_TYPE;    return TK_NODE_TYPE;  }
	YY_BREAK
case 33:
YY_RULE_SETUP
#line 247 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_NAME;         return TK_NAME;       }
	YY_BREAK
case 34:
YY_RULE_SETUP
#line 248 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_USE;          return TK_USE;        }
	YY_BREAK
case 35:
YY_RULE_SETUP
#line 249 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_GROUP;        return TK_GROUP;      }
	YY_BREAK
case 36:
YY_RULE_SETUP
#line 250 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_VLARB_HIGH;   return TK_VLARB_HIGH; }
	YY_BREAK
case 37:
YY_RULE_SETUP
#line 251 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_VLARB_LOW;    return TK_VLARB_LOW;  }
	YY_BREAK
case 38:
YY_RULE_SETUP
#line 252 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_VLARB_HIGH_LIMIT; return TK_VLARB_HIGH_LIMIT;}
	YY_BREAK
case 39:
YY_RULE_SETUP
#line 253 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_TO;           return TK_TO;         }
	YY_BREAK
case 40:
YY_RULE_SETUP
#line 254 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_FROM;         return TK_FROM;       }
	YY_BREAK
case 41:
YY_RULE_SETUP
#line 255 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ACROSS_TO;    return TK_ACROSS_TO;  }
	YY_BREAK
case 42:
YY_RULE_SETUP
#line 256 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ACROSS_FROM;  return TK_ACROSS_FROM;}
	YY_BREAK
case 43:
YY_RULE_SETUP
#line 257 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ACROSS;       return TK_ACROSS;     }
	YY_BREAK
case 44:
YY_RULE_SETUP
#line 258 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_SL2VL_TABLE;  return TK_SL2VL_TABLE;}
	YY_BREAK
case 45:
YY_RULE_SETUP
#line 259 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_SL;           return TK_SL;         }
	YY_BREAK
case 46:
YY_RULE_SETUP
#line 260 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_MTU_LIMIT;    return TK_MTU_LIMIT;  }
	YY_BREAK
case 47:
YY_RULE_SETUP
#line 261 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_RATE_LIMIT;   return TK_RATE_LIMIT; }
	YY_BREAK
case 48:
YY_RULE_SETUP
#line 262 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_PACKET_LIFE;  return TK_PACKET_LIFE;}
	YY_BREAK
case 49:
YY_RULE_SETUP
#line 263 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_PATH_BITS;    return TK_PATH_BITS;  }
	YY_BREAK
case 50:
YY_RULE_SETUP
#line 264 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_QOS_CLASS;    return TK_QOS_CLASS;  }
	YY_BREAK
case 51:
YY_RULE_SETUP
#line 265 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_SOURCE;       return TK_SOURCE;     }
	YY_BREAK
case 52:
YY_RULE_SETUP
#line 266 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_DESTINATION;  return TK_DESTINATION;}
	YY_BREAK
case 53:
YY_RULE_SETUP
#line 267 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_SERVICE_ID;   return TK_SERVICE_ID; }
	YY_BREAK
case 54:
YY_RULE_SETUP
#line 268 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_PKEY;         return TK_PKEY;       }
	YY_BREAK
case 55:
YY_RULE_SETUP
#line 269 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_QOS_LEVEL_NAME; return TK_QOS_LEVEL_NAME;}
	YY_BREAK
case 56:
YY_RULE_SETUP
#line 271 "osm_qos_parser_l.l"
{ SAVE_POS; if (in_node_type) return TK_NODE_TYPE_ROUTER; yylval = strdup(yytext); return TK_TEXT; }
	YY_BREAK
case 57:
YY_RULE_SETUP
#line 272 "osm_qos_parser_l.l"
{ SAVE_POS; if (in_node_type) return TK_NODE_TYPE_CA;     yylval = strdup(yytext); return TK_TEXT; }
	YY_BREAK
case 58:
YY_RULE_SETUP
#line 273 "osm_qos_parser_l.l"
{ SAVE_POS; if (in_node_type) return TK_NODE_TYPE_SWITCH; yylval = strdup(yytext); return TK_TEXT; }
	YY_BREAK
case 59:
YY_RULE_SETUP
#line 274 "osm_qos_parser_l.l"
{ SAVE_POS; if (in_node_type) return TK_NODE_TYPE_SELF;   yylval = strdup(yytext); return TK_TEXT; }
	YY_BREAK
case 60:
YY_RULE_SETUP
#line 275 "osm_qos_parser_l.l"
{ SAVE_POS; if (in_node_type) return TK_NODE_TYPE_ALL;    yylval = strdup(yytext); return TK_TEXT; }
	YY_BREAK
case 61:
YY_RULE_SETUP
#line 277 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_DEFAULT; return TK_ULP_DEFAULT; }
	YY_BREAK
case 62:
YY_RULE_SETUP
#line 278 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_ANY; return TK_ULP_ANY_SERVICE_ID; }
	YY_BREAK
case 63:
YY_RULE_SETUP
#line 279 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_ANY; return TK_ULP_ANY_PKEY; }
	YY_BREAK
case 64:
YY_RULE_SETUP
#line 280 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_ANY; return TK_ULP_ANY_TARGET_PORT_GUID; }
	YY_BREAK
case 65:
YY_RULE_SETUP
#line 281 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_ANY; return TK_ULP_ANY_SOURCE_PORT_GUID; }
	YY_BREAK
case 66:
YY_RULE_SETUP
#line 282 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_ANY; return TK_ULP_ANY_SOURCE_TARGET_PORT_GUID; }
	YY_BREAK
case 67:
YY_RULE_SETUP
#line 284 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_SDP_DEFAULT; return TK_ULP_SDP_DEFAULT; }
	YY_BREAK
case 68:
YY_RULE_SETUP
#line 285 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_SDP_PORT; return TK_ULP_SDP_PORT; }
	YY_BREAK
case 69:
YY_RULE_SETUP
#line 287 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_RDS_DEFAULT; return TK_ULP_RDS_DEFAULT; }
	YY_BREAK
case 70:
YY_RULE_SETUP
#line 288 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_RDS_PORT; return TK_ULP_RDS_PORT; }
	YY_BREAK
case 71:
YY_RULE_SETUP
#line 290 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_SDP_DEFAULT; return TK_ULP_ISER_DEFAULT; }
	YY_BREAK
case 72:
YY_RULE_SETUP
#line 291 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_SDP_PORT; return TK_ULP_ISER_PORT; }
	YY_BREAK
case 73:
YY_RULE_SETUP
#line 293 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_SRP_GUID; return TK_ULP_SRP_GUID; }
	YY_BREAK
case 74:
YY_RULE_SETUP
#line 295 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_IPOIB_DEFAULT; return TK_ULP_IPOIB_DEFAULT; }
	YY_BREAK
case 75:
YY_RULE_SETUP
#line 296 "osm_qos_parser_l.l"
{ SAVE_POS; HANDLE_IF_IN_DESCRIPTION; START_ULP_IPOIB_PKEY; return TK_ULP_IPOIB_PKEY; }
	YY_BREAK
case 76:
YY_RULE_SETUP
#line 298 "osm_qos_parser_l.l"
{
                        SAVE_POS;
                        yylval = strdup(yytext);
                        if (in_description || in_list_of_strings || in_single_string)
                            return TK_TEXT;
                        return TK_NUMBER;
                    }
	YY_BREAK
case 77:
YY_RULE_SETUP
#line 306 "osm_qos_parser_l.l"
{
                        SAVE_POS;
                        yylval = strdup(yytext);
                        if (in_description || in_list_of_strings || in_single_string)
                            return TK_TEXT;
                        return TK_NUMBER;
                    }
	YY_BREAK
case 78:
YY_RULE_SETUP
#line 315 "osm_qos_parser_l.l"
{
                        SAVE_POS;
                        if (in_description || in_list_of_strings || in_single_string)
                        {
                            yylval = strdup(yytext);
                            return TK_TEXT;
                        }
                        return TK_DASH;
                    }
	YY_BREAK
case 79:
YY_RULE_SETUP
#line 325 "osm_qos_parser_l.l"
{
                        SAVE_POS;
                        if (in_description || in_list_of_strings || in_single_string)
                        {
                            yylval = strdup(yytext);
                            return TK_TEXT;
                        }
                        return TK_DOTDOT;
                    }
	YY_BREAK
case 80:
YY_RULE_SETUP
#line 335 "osm_qos_parser_l.l"
{
                        SAVE_POS;
                        if (in_description)
                        {
                            yylval = strdup(yytext);
                            return TK_TEXT;
                        }
                        return TK_COMMA;
                    }
	YY_BREAK
case 81:
YY_RULE_SETUP
#line 345 "osm_qos_parser_l.l"
{
                        SAVE_POS;
                        if (in_description || in_list_of_strings || in_single_string)
                        {
                            yylval = strdup(yytext);
                            return TK_TEXT;
                        }
                        return TK_ASTERISK;
                    }
	YY_BREAK
case 82:
/* rule 82 can match eol */
YY_RULE_SETUP
#line 355 "osm_qos_parser_l.l"
{
                        SAVE_POS;
                        yylval = strdup(&yytext[1]);
                        yylval[strlen(yylval)-1] = '\0';
                        return TK_TEXT;
                    }
	YY_BREAK
case 83:
YY_RULE_SETUP
#line 362 "osm_qos_parser_l.l"
{ SAVE_POS; yylval = strdup(yytext); return TK_TEXT;}
	YY_BREAK
case YY_STATE_EOF(INITIAL):
#line 364 "osm_qos_parser_l.l"
{
                        YY_NEW_FILE;
                        yyterminate();
                    }
	YY_BREAK
case 84:
YY_RULE_SETUP
#line 368 "osm_qos_parser_l.l"
ECHO;
	YY_BREAK
#line 1698 "osm_qos_parser_l.c"

	case YY_END_OF_BUFFER:
		{
		/* Amount of text matched not including the EOB char. */
		int yy_amount_of_matched_text = (int) (yy_cp - (yytext_ptr)) - 1;

		/* Undo the effects of YY_DO_BEFORE_ACTION. */
		*yy_cp = (yy_hold_char);
		YY_RESTORE_YY_MORE_OFFSET

		if ( YY_CURRENT_BUFFER_LVALUE->yy_buffer_status == YY_BUFFER_NEW )
			{
			/* We're scanning a new file or input source.  It's
			 * possible that this happened because the user
			 * just pointed yyin at a new source and called
			 * yylex().  If so, then we have to assure
			 * consistency between YY_CURRENT_BUFFER and our
			 * globals.  Here is the right place to do so, because
			 * this is the first action (other than possibly a
			 * back-up) that will match for the new input source.
			 */
			(yy_n_chars) = YY_CURRENT_BUFFER_LVALUE->yy_n_chars;
			YY_CURRENT_BUFFER_LVALUE->yy_input_file = yyin;
			YY_CURRENT_BUFFER_LVALUE->yy_buffer_status = YY_BUFFER_NORMAL;
			}

		/* Note that here we test for yy_c_buf_p "<=" to the position
		 * of the first EOB in the buffer, since yy_c_buf_p will
		 * already have been incremented past the NUL character
		 * (since all states make transitions on EOB to the
		 * end-of-buffer state).  Contrast this with the test
		 * in input().
		 */
		if ( (yy_c_buf_p) <= &YY_CURRENT_BUFFER_LVALUE->yy_ch_buf[(yy_n_chars)] )
			{ /* This was really a NUL. */
			yy_state_type yy_next_state;

			(yy_c_buf_p) = (yytext_ptr) + yy_amount_of_matched_text;

			yy_current_state = yy_get_previous_state(  );

			/* Okay, we're now positioned to make the NUL
			 * transition.  We couldn't have
			 * yy_get_previous_state() go ahead and do it
			 * for us because it doesn't know how to deal
			 * with the possibility of jamming (and we don't
			 * want to build jamming into it because then it
			 * will run more slowly).
			 */

			yy_next_state = yy_try_NUL_trans( yy_current_state );

			yy_bp = (yytext_ptr) + YY_MORE_ADJ;

			if ( yy_next_state )
				{
				/* Consume the NUL. */
				yy_cp = ++(yy_c_buf_p);
				yy_current_state = yy_next_state;
				goto yy_match;
				}

			else
				{
				yy_cp = (yy_c_buf_p);
				goto yy_find_action;
				}
			}

		else switch ( yy_get_next_buffer(  ) )
			{
			case EOB_ACT_END_OF_FILE:
				{
				(yy_did_buffer_switch_on_eof) = 0;

				if ( yywrap( ) )
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
					(yy_c_buf_p) = (yytext_ptr) + YY_MORE_ADJ;

					yy_act = YY_STATE_EOF(YY_START);
					goto do_action;
					}

				else
					{
					if ( ! (yy_did_buffer_switch_on_eof) )
						YY_NEW_FILE;
					}
				break;
				}

			case EOB_ACT_CONTINUE_SCAN:
				(yy_c_buf_p) =
					(yytext_ptr) + yy_amount_of_matched_text;

				yy_current_state = yy_get_previous_state(  );

				yy_cp = (yy_c_buf_p);
				yy_bp = (yytext_ptr) + YY_MORE_ADJ;
				goto yy_match;

			case EOB_ACT_LAST_MATCH:
				(yy_c_buf_p) =
				&YY_CURRENT_BUFFER_LVALUE->yy_ch_buf[(yy_n_chars)];

				yy_current_state = yy_get_previous_state(  );

				yy_cp = (yy_c_buf_p);
				yy_bp = (yytext_ptr) + YY_MORE_ADJ;
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