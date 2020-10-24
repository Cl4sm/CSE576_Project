{
	register yy_state_type yy_current_state;
	register char *yy_cp, *yy_bp;
	register int yy_act;
    
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

	{
#line 80 "conf_lexer.l"

#line 2031 "conf_lexer.c"

	while ( 1 )		/* loops until end-of-file is reached */
		{
		(yy_more_len) = 0;
		if ( (yy_more_flag) )
			{
			(yy_more_len) = (yy_c_buf_p) - (yytext_ptr);
			(yy_more_flag) = 0;
			}
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
			register YY_CHAR yy_c = yy_ec[YY_SC_TO_UI(*yy_cp)] ;
			if ( yy_accept[yy_current_state] )
				{
				(yy_last_accepting_state) = yy_current_state;
				(yy_last_accepting_cpos) = yy_cp;
				}
			while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
				{
				yy_current_state = (int) yy_def[yy_current_state];
				if ( yy_current_state >= 1707 )
					yy_c = yy_meta[(unsigned int) yy_c];
				}
			yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
			++yy_cp;
			}
		while ( yy_current_state != 1706 );
		yy_cp = (yy_last_accepting_cpos);
		yy_current_state = (yy_last_accepting_state);

yy_find_action:
		yy_act = yy_accept[yy_current_state];

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
YY_RULE_SETUP
#line 81 "conf_lexer.l"
{ cinclude(); }
	YY_BREAK
case 2:
YY_RULE_SETUP
#line 82 "conf_lexer.l"
{ ccomment(); }
	YY_BREAK
case 3:
/* rule 3 can match eol */
YY_RULE_SETUP
#line 83 "conf_lexer.l"
{ strlcpy(linebuf, yytext + 1, sizeof(linebuf)); ++lineno; yyless(1); }
	YY_BREAK
case 4:
YY_RULE_SETUP
#line 84 "conf_lexer.l"
;
	YY_BREAK
case 5:
YY_RULE_SETUP
#line 85 "conf_lexer.l"
;
	YY_BREAK
case 6:
YY_RULE_SETUP
#line 86 "conf_lexer.l"
{ yylval.number = atoi(yytext); return NUMBER; }
	YY_BREAK
case 7:
/* rule 7 can match eol */
YY_RULE_SETUP
#line 87 "conf_lexer.l"
{ if (yytext[yyleng - 2] == '\\')
                  {
                    yyless(yyleng - 1);  /* Return last quote */
                    yymore();  /* Append next string */
                  }
                  else
                  {
                    yylval.string = yytext + 1;

                    if (yylval.string[yyleng - 2] != '"')
                      ilog(LOG_TYPE_IRCD, "Unterminated character string");
                    else
                    {
                      unsigned int i = 0, j = 0;

                      yylval.string[yyleng - 2] = '\0';  /* Remove close quote */

                      for (; yylval.string[i] != '\0'; ++i, ++j)
                      {
                        if (yylval.string[i] != '\\')
                          yylval.string[j] = yylval.string[i];
                        else
                        {
                          ++i;

                          if (yylval.string[i] == '\0')  /* XXX: should not happen */
                          {
                            ilog(LOG_TYPE_IRCD, "Unterminated character string");
                            break;
                          }

                          yylval.string[j] = yylval.string[i];
                        }
                      }

                      yylval.string[j] = '\0';
                      return QSTRING;
                    }
                  }
                }
	YY_BREAK
case 8:
YY_RULE_SETUP
#line 128 "conf_lexer.l"
{ return ACCEPT_PASSWORD; }
	YY_BREAK
case 9:
YY_RULE_SETUP
#line 129 "conf_lexer.l"
{ return ADMIN; }
	YY_BREAK
case 10:
YY_RULE_SETUP
#line 130 "conf_lexer.l"
{ return ADMIN; }
	YY_BREAK
case 11:
YY_RULE_SETUP
#line 131 "conf_lexer.l"
{ return AFTYPE; }
	YY_BREAK
case 12:
YY_RULE_SETUP
#line 132 "conf_lexer.l"
{ return T_ALL; }
	YY_BREAK
case 13:
YY_RULE_SETUP
#line 133 "conf_lexer.l"
{ return ANTI_NICK_FLOOD; }
	YY_BREAK
case 14:
YY_RULE_SETUP
#line 134 "conf_lexer.l"
{ return ANTI_SPAM_EXIT_MESSAGE_TIME; }
	YY_BREAK
case 15:
YY_RULE_SETUP
#line 135 "conf_lexer.l"
{ return IRCD_AUTH; }
	YY_BREAK
case 16:
YY_RULE_SETUP
#line 136 "conf_lexer.l"
{ return AUTOCONN; }
	YY_BREAK
case 17:
YY_RULE_SETUP
#line 137 "conf_lexer.l"
{ return AWAY_COUNT; }
	YY_BREAK
case 18:
YY_RULE_SETUP
#line 138 "conf_lexer.l"
{ return AWAY_TIME; }
	YY_BREAK
case 19:
YY_RULE_SETUP
#line 139 "conf_lexer.l"
{ return T_BOTS; }
	YY_BREAK
case 20:
YY_RULE_SETUP
#line 140 "conf_lexer.l"
{ return CALLER_ID_WAIT; }
	YY_BREAK
case 21:
YY_RULE_SETUP
#line 141 "conf_lexer.l"
{ return T_CALLERID; }
	YY_BREAK
case 22:
YY_RULE_SETUP
#line 142 "conf_lexer.l"
{ return CAN_FLOOD; }
	YY_BREAK
case 23:
YY_RULE_SETUP
#line 143 "conf_lexer.l"
{ return T_CCONN; }
	YY_BREAK
case 24:
YY_RULE_SETUP
#line 144 "conf_lexer.l"
{ return CHANNEL; }
	YY_BREAK
case 25:
YY_RULE_SETUP
#line 145 "conf_lexer.l"
{ return CIDR_BITLEN_IPV4; }
	YY_BREAK
case 26:
YY_RULE_SETUP
#line 146 "conf_lexer.l"
{ return CIDR_BITLEN_IPV6; }
	YY_BREAK
case 27:
YY_RULE_SETUP
#line 147 "conf_lexer.l"
{ return CLASS; }
	YY_BREAK
case 28:
YY_RULE_SETUP
#line 148 "conf_lexer.l"
{ return T_CLUSTER; }
	YY_BREAK
case 29:
YY_RULE_SETUP
#line 149 "conf_lexer.l"
{ return T_COMMAND; }
	YY_BREAK
case 30:
YY_RULE_SETUP
#line 150 "conf_lexer.l"
{ return CONNECT; }
	YY_BREAK
case 31:
YY_RULE_SETUP
#line 151 "conf_lexer.l"
{ return CONNECTFREQ; }
	YY_BREAK
case 32:
YY_RULE_SETUP
#line 152 "conf_lexer.l"
{ return CYCLE_ON_HOST_CHANGE; }
	YY_BREAK
case 33:
YY_RULE_SETUP
#line 153 "conf_lexer.l"
{ return T_DEAF; }
	YY_BREAK
case 34:
YY_RULE_SETUP
#line 154 "conf_lexer.l"
{ return T_DEBUG; }
	YY_BREAK
case 35:
YY_RULE_SETUP
#line 155 "conf_lexer.l"
{ return DEFAULT_FLOODCOUNT; }
	YY_BREAK
case 36:
YY_RULE_SETUP
#line 156 "conf_lexer.l"
{ return DEFAULT_SPLIT_SERVER_COUNT; }
	YY_BREAK
case 37:
YY_RULE_SETUP
#line 157 "conf_lexer.l"
{ return DEFAULT_SPLIT_USER_COUNT; }
	YY_BREAK
case 38:
YY_RULE_SETUP
#line 158 "conf_lexer.l"
{ return DENY; }
	YY_BREAK
case 39:
YY_RULE_SETUP
#line 159 "conf_lexer.l"
{ return DESCRIPTION; }
	YY_BREAK
case 40:
YY_RULE_SETUP
#line 160 "conf_lexer.l"
{ return DIE; }
	YY_BREAK
case 41:
YY_RULE_SETUP
#line 161 "conf_lexer.l"
{ return DISABLE_AUTH; }
	YY_BREAK
case 42:
YY_RULE_SETUP
#line 162 "conf_lexer.l"
{ return DISABLE_FAKE_CHANNELS; }
	YY_BREAK
case 43:
YY_RULE_SETUP
#line 163 "conf_lexer.l"
{ return DISABLE_REMOTE_COMMANDS; }
	YY_BREAK
case 44:
YY_RULE_SETUP
#line 164 "conf_lexer.l"
{ return T_DLINE; }
	YY_BREAK
case 45:
YY_RULE_SETUP
#line 165 "conf_lexer.l"
{ return DOTS_IN_IDENT; }
	YY_BREAK
case 46:
YY_RULE_SETUP
#line 166 "conf_lexer.l"
{ return EGDPOOL_PATH; }
	YY_BREAK
case 47:
YY_RULE_SETUP
#line 167 "conf_lexer.l"
{ return EMAIL; }
	YY_BREAK
case 48:
YY_RULE_SETUP
#line 168 "conf_lexer.l"
{ return ENCRYPTED; }
	YY_BREAK
case 49:
YY_RULE_SETUP
#line 169 "conf_lexer.l"
{ return EXCEED_LIMIT; }
	YY_BREAK
case 50:
YY_RULE_SETUP
#line 170 "conf_lexer.l"
{ return EXEMPT; }
	YY_BREAK
case 51:
YY_RULE_SETUP
#line 171 "conf_lexer.l"
{ return T_EXTERNAL; }
	YY_BREAK
case 52:
YY_RULE_SETUP
#line 172 "conf_lexer.l"
{ return FAILED_OPER_NOTICE; }
	YY_BREAK
case 53:
YY_RULE_SETUP
#line 173 "conf_lexer.l"
{ return T_FARCONNECT; }
	YY_BREAK
case 54:
YY_RULE_SETUP
#line 174 "conf_lexer.l"
{ return T_FILE; }
	YY_BREAK
case 55:
YY_RULE_SETUP
#line 175 "conf_lexer.l"
{ return IRCD_FLAGS; }
	YY_BREAK
case 56:
YY_RULE_SETUP
#line 176 "conf_lexer.l"
{ return FLATTEN_LINKS; }
	YY_BREAK
case 57:
YY_RULE_SETUP
#line 177 "conf_lexer.l"
{ return T_FULL; }
	YY_BREAK
case 58:
YY_RULE_SETUP
#line 178 "conf_lexer.l"
{ return GECOS; }
	YY_BREAK
case 59:
YY_RULE_SETUP
#line 179 "conf_lexer.l"
{ return GENERAL; }
	YY_BREAK
case 60:
YY_RULE_SETUP
#line 180 "conf_lexer.l"
{ return GLINE; }
	YY_BREAK
case 61:
YY_RULE_SETUP
#line 181 "conf_lexer.l"
{ return GLINE_DURATION; }
	YY_BREAK
case 62:
YY_RULE_SETUP
#line 182 "conf_lexer.l"
{ return GLINE_ENABLE; }
	YY_BREAK
case 63:
YY_RULE_SETUP
#line 183 "conf_lexer.l"
{ return GLINE_EXEMPT; }
	YY_BREAK
case 64:
YY_RULE_SETUP
#line 184 "conf_lexer.l"
{ return GLINE_MIN_CIDR; }
	YY_BREAK
case 65:
YY_RULE_SETUP
#line 185 "conf_lexer.l"
{ return GLINE_MIN_CIDR6; }
	YY_BREAK
case 66:
YY_RULE_SETUP
#line 186 "conf_lexer.l"
{ return GLINE_REQUEST_DURATION; }
	YY_BREAK
case 67:
YY_RULE_SETUP
#line 187 "conf_lexer.l"
{ return GLOBAL_KILL; }
	YY_BREAK
case 68:
YY_RULE_SETUP
#line 188 "conf_lexer.l"
{ return T_GLOBOPS; }
	YY_BREAK
case 69:
YY_RULE_SETUP
#line 189 "conf_lexer.l"
{ return NEED_IDENT; }
	YY_BREAK
case 70:
YY_RULE_SETUP
#line 190 "conf_lexer.l"
{ return HAVENT_READ_CONF; }
	YY_BREAK
case 71:
YY_RULE_SETUP
#line 191 "conf_lexer.l"
{ return HIDDEN; }
	YY_BREAK
case 72:
YY_RULE_SETUP
#line 192 "conf_lexer.l"
{ return HIDDEN_NAME; }
	YY_BREAK
case 73:
YY_RULE_SETUP
#line 193 "conf_lexer.l"
{ return HIDE_CHANS; }
	YY_BREAK
case 74:
YY_RULE_SETUP
#line 194 "conf_lexer.l"
{ return HIDE_IDLE; }
	YY_BREAK
case 75:
YY_RULE_SETUP
#line 195 "conf_lexer.l"
{ return HIDE_IDLE_FROM_OPERS; }
	YY_BREAK
case 76:
YY_RULE_SETUP
#line 196 "conf_lexer.l"
{ return HIDE_SERVER_IPS; }
	YY_BREAK
case 77:
YY_RULE_SETUP
#line 197 "conf_lexer.l"
{ return HIDE_SERVERS; }
	YY_BREAK
case 78:
YY_RULE_SETUP
#line 198 "conf_lexer.l"
{ return HIDE_SERVICES; }
	YY_BREAK
case 79:
YY_RULE_SETUP
#line 199 "conf_lexer.l"
{ return HIDE_SPOOF_IPS; }
	YY_BREAK
case 80:
YY_RULE_SETUP
#line 200 "conf_lexer.l"
{ return HOST; }
	YY_BREAK
case 81:
YY_RULE_SETUP
#line 201 "conf_lexer.l"
{ return HUB; }
	YY_BREAK
case 82:
YY_RULE_SETUP
#line 202 "conf_lexer.l"
{ return HUB_MASK; }
	YY_BREAK
case 83:
YY_RULE_SETUP
#line 203 "conf_lexer.l"
{ return IGNORE_BOGUS_TS; }
	YY_BREAK
case 84:
YY_RULE_SETUP
#line 204 "conf_lexer.l"
{ return T_INVISIBLE; }
	YY_BREAK
case 85:
YY_RULE_SETUP
#line 205 "conf_lexer.l"
{ return INVISIBLE_ON_CONNECT; }
	YY_BREAK
case 86:
YY_RULE_SETUP
#line 206 "conf_lexer.l"
{ return INVITE_CLIENT_COUNT; }
	YY_BREAK
case 87:
YY_RULE_SETUP
#line 207 "conf_lexer.l"
{ return INVITE_CLIENT_TIME; }
	YY_BREAK
case 88:
YY_RULE_SETUP
#line 208 "conf_lexer.l"
{ return IP; }
	YY_BREAK
case 89:
YY_RULE_SETUP
#line 209 "conf_lexer.l"
{ return T_IPV4; }
	YY_BREAK
case 90:
YY_RULE_SETUP
#line 210 "conf_lexer.l"
{ return T_IPV6; }
	YY_BREAK
case 91:
YY_RULE_SETUP
#line 211 "conf_lexer.l"
{ return JOIN_FLOOD_COUNT; }
	YY_BREAK
case 92:
YY_RULE_SETUP
#line 212 "conf_lexer.l"
{ return JOIN_FLOOD_TIME; }
	YY_BREAK
case 93:
YY_RULE_SETUP
#line 213 "conf_lexer.l"
{ return KILL; }
	YY_BREAK
case 94:
YY_RULE_SETUP
#line 214 "conf_lexer.l"
{ return KILL_CHASE_TIME_LIMIT; }
	YY_BREAK
case 95:
YY_RULE_SETUP
#line 215 "conf_lexer.l"
{ return KLINE; }
	YY_BREAK
case 96:
YY_RULE_SETUP
#line 216 "conf_lexer.l"
{ return KLINE_EXEMPT; }
	YY_BREAK
case 97:
YY_RULE_SETUP
#line 217 "conf_lexer.l"
{ return KNOCK_CLIENT_COUNT; }
	YY_BREAK
case 98:
YY_RULE_SETUP
#line 218 "conf_lexer.l"
{ return KNOCK_CLIENT_TIME; }
	YY_BREAK
case 99:
YY_RULE_SETUP
#line 219 "conf_lexer.l"
{ return KNOCK_DELAY_CHANNEL; }
	YY_BREAK
case 100:
YY_RULE_SETUP
#line 220 "conf_lexer.l"
{ return LEAF_MASK; }
	YY_BREAK
case 101:
YY_RULE_SETUP
#line 221 "conf_lexer.l"
{ return LINKS_DELAY; }
	YY_BREAK
case 102:
YY_RULE_SETUP
#line 222 "conf_lexer.l"
{ return LISTEN; }
	YY_BREAK
case 103:
YY_RULE_SETUP
#line 223 "conf_lexer.l"
{ return T_LOCOPS; }
	YY_BREAK
case 104:
YY_RULE_SETUP
#line 224 "conf_lexer.l"
{ return T_LOG; }
	YY_BREAK
case 105:
YY_RULE_SETUP
#line 225 "conf_lexer.l"
{ return MASK; }
	YY_BREAK
case 106:
YY_RULE_SETUP
#line 226 "conf_lexer.l"
{ return TMASKED; }
	YY_BREAK
case 107:
YY_RULE_SETUP
#line 227 "conf_lexer.l"
{ return MAX_ACCEPT; }
	YY_BREAK
case 108:
YY_RULE_SETUP
#line 228 "conf_lexer.l"
{ return MAX_BANS; }
	YY_BREAK
case 109:
YY_RULE_SETUP
#line 229 "conf_lexer.l"
{ return MAX_CHANNELS; }
	YY_BREAK
case 110:
YY_RULE_SETUP
#line 230 "conf_lexer.l"
{ return T_MAX_CLIENTS; }
	YY_BREAK
case 111:
YY_RULE_SETUP
#line 231 "conf_lexer.l"
{ return MAX_GLOBAL; }
	YY_BREAK
case 112:
YY_RULE_SETUP
#line 232 "conf_lexer.l"
{ return MAX_IDENT; }
	YY_BREAK
case 113:
YY_RULE_SETUP
#line 233 "conf_lexer.l"
{ return MAX_IDLE; }
	YY_BREAK
case 114:
YY_RULE_SETUP
#line 234 "conf_lexer.l"
{ return MAX_LOCAL; }
	YY_BREAK
case 115:
YY_RULE_SETUP
#line 235 "conf_lexer.l"
{ return MAX_NICK_CHANGES; }
	YY_BREAK
case 116:
YY_RULE_SETUP
#line 236 "conf_lexer.l"
{ return MAX_NICK_LENGTH; }
	YY_BREAK
case 117:
YY_RULE_SETUP
#line 237 "conf_lexer.l"
{ return MAX_NICK_TIME; }
	YY_BREAK
case 118:
YY_RULE_SETUP
#line 238 "conf_lexer.l"
{ return MAX_NUMBER; }
	YY_BREAK
case 119:
YY_RULE_SETUP
#line 239 "conf_lexer.l"
{ return MAX_TARGETS; }
	YY_BREAK
case 120:
YY_RULE_SETUP
#line 240 "conf_lexer.l"
{ return MAX_TOPIC_LENGTH; }
	YY_BREAK
case 121:
YY_RULE_SETUP
#line 241 "conf_lexer.l"
{ return MAX_WATCH; }
	YY_BREAK
case 122:
YY_RULE_SETUP
#line 242 "conf_lexer.l"
{ return MIN_IDLE; }
	YY_BREAK
case 123:
YY_RULE_SETUP
#line 243 "conf_lexer.l"
{ return MIN_NONWILDCARD; }
	YY_BREAK
case 124:
YY_RULE_SETUP
#line 244 "conf_lexer.l"
{ return MIN_NONWILDCARD_SIMPLE; }
	YY_BREAK
case 125:
YY_RULE_SETUP
#line 245 "conf_lexer.l"
{ return MODULE; }
	YY_BREAK
case 126:
YY_RULE_SETUP
#line 246 "conf_lexer.l"
{ return MODULES; }
	YY_BREAK
case 127:
YY_RULE_SETUP
#line 247 "conf_lexer.l"
{ return MOTD; }
	YY_BREAK
case 128:
YY_RULE_SETUP
#line 248 "conf_lexer.l"
{ return NAME; }
	YY_BREAK
case 129:
YY_RULE_SETUP
#line 249 "conf_lexer.l"
{ return T_NCHANGE; }
	YY_BREAK
case 130:
YY_RULE_SETUP
#line 250 "conf_lexer.l"
{ return NEED_IDENT; }
	YY_BREAK
case 131:
YY_RULE_SETUP
#line 251 "conf_lexer.l"
{ return NEED_PASSWORD; }
	YY_BREAK
case 132:
YY_RULE_SETUP
#line 252 "conf_lexer.l"
{ return NETWORK_DESC; }
	YY_BREAK
case 133:
YY_RULE_SETUP
#line 253 "conf_lexer.l"
{ return NETWORK_NAME; }
	YY_BREAK
case 134:
YY_RULE_SETUP
#line 254 "conf_lexer.l"
{ return NICK; }
	YY_BREAK
case 135:
YY_RULE_SETUP
#line 255 "conf_lexer.l"
{ return NO_CREATE_ON_SPLIT; }
	YY_BREAK
case 136:
YY_RULE_SETUP
#line 256 "conf_lexer.l"
{ return NO_JOIN_ON_SPLIT; }
	YY_BREAK
case 137:
YY_RULE_SETUP
#line 257 "conf_lexer.l"
{ return NO_OPER_FLOOD; }
	YY_BREAK
case 138:
YY_RULE_SETUP
#line 258 "conf_lexer.l"
{ return NO_TILDE; }
	YY_BREAK
case 139:
YY_RULE_SETUP
#line 259 "conf_lexer.l"
{ return T_NONONREG; }
	YY_BREAK
case 140:
YY_RULE_SETUP
#line 260 "conf_lexer.l"
{ return NUMBER_PER_CIDR; }
	YY_BREAK
case 141:
YY_RULE_SETUP
#line 261 "conf_lexer.l"
{ return NUMBER_PER_IP; }
	YY_BREAK
case 142:
YY_RULE_SETUP
#line 262 "conf_lexer.l"
{ return OPERATOR; }
	YY_BREAK
case 143:
YY_RULE_SETUP
#line 263 "conf_lexer.l"
{ return OPER_ONLY_UMODES; }
	YY_BREAK
case 144:
YY_RULE_SETUP
#line 264 "conf_lexer.l"
{ return OPER_PASS_RESV; }
	YY_BREAK
case 145:
YY_RULE_SETUP
#line 265 "conf_lexer.l"
{ return OPER_UMODES; }
	YY_BREAK
case 146:
YY_RULE_SETUP
#line 266 "conf_lexer.l"
{ return OPERATOR; }
	YY_BREAK
case 147:
YY_RULE_SETUP
#line 267 "conf_lexer.l"
{ return OPERS_BYPASS_CALLERID; }
	YY_BREAK
case 148:
YY_RULE_SETUP
#line 268 "conf_lexer.l"
{ return PACE_WAIT; }
	YY_BREAK
case 149:
YY_RULE_SETUP
#line 269 "conf_lexer.l"
{ return PACE_WAIT_SIMPLE; }
	YY_BREAK
case 150:
YY_RULE_SETUP
#line 270 "conf_lexer.l"
{ return PASSWORD; }
	YY_BREAK
case 151:
YY_RULE_SETUP
#line 271 "conf_lexer.l"
{ return PASSWORD; }
	YY_BREAK
case 152:
YY_RULE_SETUP
#line 272 "conf_lexer.l"
{ return PATH; }
	YY_BREAK
case 153:
YY_RULE_SETUP
#line 273 "conf_lexer.l"
{ return PING_COOKIE; }
	YY_BREAK
case 154:
YY_RULE_SETUP
#line 274 "conf_lexer.l"
{ return PING_TIME; }
	YY_BREAK
case 155:
YY_RULE_SETUP
#line 275 "conf_lexer.l"
{ return PORT; }
	YY_BREAK
case 156:
YY_RULE_SETUP
#line 276 "conf_lexer.l"
{ return T_PREPEND; }
	YY_BREAK
case 157:
YY_RULE_SETUP
#line 277 "conf_lexer.l"
{ return T_PSEUDO; }
	YY_BREAK
case 158:
YY_RULE_SETUP
#line 278 "conf_lexer.l"
{ return RESV; }
	YY_BREAK
case 159:
YY_RULE_SETUP
#line 279 "conf_lexer.l"
{ return RANDOM_IDLE; }
	YY_BREAK
case 160:
YY_RULE_SETUP
#line 280 "conf_lexer.l"
{ return REASON; }
	YY_BREAK
case 161:
YY_RULE_SETUP
#line 281 "conf_lexer.l"
{ return T_RECVQ; }
	YY_BREAK
case 162:
YY_RULE_SETUP
#line 282 "conf_lexer.l"
{ return REDIRPORT; }
	YY_BREAK
case 163:
YY_RULE_SETUP
#line 283 "conf_lexer.l"
{ return REDIRSERV; }
	YY_BREAK
case 164:
YY_RULE_SETUP
#line 284 "conf_lexer.l"
{ return REHASH; }
	YY_BREAK
case 165:
YY_RULE_SETUP
#line 285 "conf_lexer.l"
{ return T_REJ; }
	YY_BREAK
case 166:
YY_RULE_SETUP
#line 286 "conf_lexer.l"
{ return REMOTE; }
	YY_BREAK
case 167:
YY_RULE_SETUP
#line 287 "conf_lexer.l"
{ return REMOTEBAN; }
	YY_BREAK
case 168:
YY_RULE_SETUP
#line 288 "conf_lexer.l"
{ return T_RESTART; }
	YY_BREAK
case 169:
YY_RULE_SETUP
#line 289 "conf_lexer.l"
{ return RESV; }
	YY_BREAK
case 170:
YY_RULE_SETUP
#line 290 "conf_lexer.l"
{ return RESV_EXEMPT; }
	YY_BREAK
case 171:
YY_RULE_SETUP
#line 291 "conf_lexer.l"
{ return RSA_PRIVATE_KEY_FILE; }
	YY_BREAK
case 172:
YY_RULE_SETUP
#line 292 "conf_lexer.l"
{ return RSA_PUBLIC_KEY_FILE; }
	YY_BREAK
case 173:
YY_RULE_SETUP
#line 293 "conf_lexer.l"
{ return SEND_PASSWORD; }
	YY_BREAK
case 174:
YY_RULE_SETUP
#line 294 "conf_lexer.l"
{ return SENDQ; }
	YY_BREAK
case 175:
YY_RULE_SETUP
#line 295 "conf_lexer.l"
{ return T_SERVER; }
	YY_BREAK
case 176:
YY_RULE_SETUP
#line 296 "conf_lexer.l"
{ return SERVERHIDE; }
	YY_BREAK
case 177:
YY_RULE_SETUP
#line 297 "conf_lexer.l"
{ return SERVERINFO; }
	YY_BREAK
case 178:
YY_RULE_SETUP
#line 298 "conf_lexer.l"
{ return T_SERVICE; }
	YY_BREAK
case 179:
YY_RULE_SETUP
#line 299 "conf_lexer.l"
{ return T_SERVNOTICE; }
	YY_BREAK
case 180:
YY_RULE_SETUP
#line 300 "conf_lexer.l"
{ return T_SET; }
	YY_BREAK
case 181:
YY_RULE_SETUP
#line 301 "conf_lexer.l"
{ return T_SHARED; }
	YY_BREAK
case 182:
YY_RULE_SETUP
#line 302 "conf_lexer.l"
{ return SHORT_MOTD; }
	YY_BREAK
case 183:
YY_RULE_SETUP
#line 303 "conf_lexer.l"
{ return IRCD_SID; }
	YY_BREAK
case 184:
YY_RULE_SETUP
#line 304 "conf_lexer.l"
{ return T_SIZE; }
	YY_BREAK
case 185:
YY_RULE_SETUP
#line 305 "conf_lexer.l"
{ return T_SKILL; }
	YY_BREAK
case 186:
YY_RULE_SETUP
#line 306 "conf_lexer.l"
{ return T_SOFTCALLERID; }
	YY_BREAK
case 187:
YY_RULE_SETUP
#line 307 "conf_lexer.l"
{ return SPOOF; }
	YY_BREAK
case 188:
YY_RULE_SETUP
#line 308 "conf_lexer.l"
{ return SPOOF_NOTICE; }
	YY_BREAK
case 189:
YY_RULE_SETUP
#line 309 "conf_lexer.l"
{ return T_SPY; }
	YY_BREAK
case 190:
YY_RULE_SETUP
#line 310 "conf_lexer.l"
{ return SQUIT; }
	YY_BREAK
case 191:
YY_RULE_SETUP
#line 311 "conf_lexer.l"
{ return T_SSL; }
	YY_BREAK
case 192:
YY_RULE_SETUP
#line 312 "conf_lexer.l"
{ return SSL_CERTIFICATE_FILE; }
	YY_BREAK
case 193:
YY_RULE_SETUP
#line 313 "conf_lexer.l"
{ return SSL_CERTIFICATE_FINGERPRINT; }
	YY_BREAK
case 194:
YY_RULE_SETUP
#line 314 "conf_lexer.l"
{ return T_SSL_CIPHER_LIST; }
	YY_BREAK
case 195:
YY_RULE_SETUP
#line 315 "conf_lexer.l"
{ return SSL_CONNECTION_REQUIRED; }
	YY_BREAK
case 196:
YY_RULE_SETUP
#line 316 "conf_lexer.l"
{ return SSL_DH_ELLIPTIC_CURVE; }
	YY_BREAK
case 197:
YY_RULE_SETUP
#line 317 "conf_lexer.l"
{ return SSL_DH_PARAM_FILE; }
	YY_BREAK
case 198:
YY_RULE_SETUP
#line 318 "conf_lexer.l"
{ return SSL_MESSAGE_DIGEST_ALGORITHM; }
	YY_BREAK
case 199:
YY_RULE_SETUP
#line 319 "conf_lexer.l"
{ return STATS_E_DISABLED; }
	YY_BREAK
case 200:
YY_RULE_SETUP
#line 320 "conf_lexer.l"
{ return STATS_I_OPER_ONLY; }
	YY_BREAK
case 201:
YY_RULE_SETUP
#line 321 "conf_lexer.l"
{ return STATS_K_OPER_ONLY; }
	YY_BREAK
case 202:
YY_RULE_SETUP
#line 322 "conf_lexer.l"
{ return STATS_O_OPER_ONLY; }
	YY_BREAK
case 203:
YY_RULE_SETUP
#line 323 "conf_lexer.l"
{ return STATS_P_OPER_ONLY; }
	YY_BREAK
case 204:
YY_RULE_SETUP
#line 324 "conf_lexer.l"
{ return STATS_U_OPER_ONLY; }
	YY_BREAK
case 205:
YY_RULE_SETUP
#line 325 "conf_lexer.l"
{ return T_TARGET; }
	YY_BREAK
case 206:
YY_RULE_SETUP
#line 326 "conf_lexer.l"
{ return THROTTLE_COUNT; }
	YY_BREAK
case 207:
YY_RULE_SETUP
#line 327 "conf_lexer.l"
{ return THROTTLE_TIME; }
	YY_BREAK
case 208:
YY_RULE_SETUP
#line 328 "conf_lexer.l"
{ return TKLINE_EXPIRE_NOTICES; }
	YY_BREAK
case 209:
YY_RULE_SETUP
#line 329 "conf_lexer.l"
{ return TRUE_NO_OPER_FLOOD; }
	YY_BREAK
case 210:
YY_RULE_SETUP
#line 330 "conf_lexer.l"
{ return TS_MAX_DELTA; }
	YY_BREAK
case 211:
YY_RULE_SETUP
#line 331 "conf_lexer.l"
{ return TS_WARN_DELTA; }
	YY_BREAK
case 212:
YY_RULE_SETUP
#line 332 "conf_lexer.l"
{ return TYPE; }
	YY_BREAK
case 213:
YY_RULE_SETUP
#line 333 "conf_lexer.l"
{ return T_UMODES; }
	YY_BREAK
case 214:
YY_RULE_SETUP
#line 334 "conf_lexer.l"
{ return T_UNAUTH; }
	YY_BREAK
case 215:
YY_RULE_SETUP
#line 335 "conf_lexer.l"
{ return T_UNDLINE; }
	YY_BREAK
case 216:
YY_RULE_SETUP
#line 336 "conf_lexer.l"
{ return UNKLINE; }
	YY_BREAK
case 217:
YY_RULE_SETUP
#line 337 "conf_lexer.l"
{ return T_UNLIMITED; }
	YY_BREAK
case 218:
YY_RULE_SETUP
#line 338 "conf_lexer.l"
{ return T_UNRESV; }
	YY_BREAK
case 219:
YY_RULE_SETUP
#line 339 "conf_lexer.l"
{ return T_UNXLINE; }
	YY_BREAK
case 220:
YY_RULE_SETUP
#line 340 "conf_lexer.l"
{ return USE_EGD; }
	YY_BREAK
case 221:
YY_RULE_SETUP
#line 341 "conf_lexer.l"
{ return USE_LOGGING; }
	YY_BREAK
case 222:
YY_RULE_SETUP
#line 342 "conf_lexer.l"
{ return USER; }
	YY_BREAK
case 223:
YY_RULE_SETUP
#line 343 "conf_lexer.l"
{ return VHOST; }
	YY_BREAK
case 224:
YY_RULE_SETUP
#line 344 "conf_lexer.l"
{ return VHOST6; }
	YY_BREAK
case 225:
YY_RULE_SETUP
#line 345 "conf_lexer.l"
{ return T_WALLOP; }
	YY_BREAK
case 226:
YY_RULE_SETUP
#line 346 "conf_lexer.l"
{ return T_WALLOPS; }
	YY_BREAK
case 227:
YY_RULE_SETUP
#line 347 "conf_lexer.l"
{ return WARN_NO_CONNECT_BLOCK; }
	YY_BREAK
case 228:
YY_RULE_SETUP
#line 348 "conf_lexer.l"
{ return T_WEBIRC; }
	YY_BREAK
case 229:
YY_RULE_SETUP
#line 349 "conf_lexer.l"
{ return XLINE; }
	YY_BREAK
case 230:
YY_RULE_SETUP
#line 351 "conf_lexer.l"
{ yylval.number = 1; return TBOOL; }
	YY_BREAK
case 231:
YY_RULE_SETUP
#line 352 "conf_lexer.l"
{ yylval.number = 0; return TBOOL; }
	YY_BREAK
case 232:
YY_RULE_SETUP
#line 354 "conf_lexer.l"
{ return YEARS; }
	YY_BREAK
case 233:
YY_RULE_SETUP
#line 355 "conf_lexer.l"
{ return YEARS; }
	YY_BREAK
case 234:
YY_RULE_SETUP
#line 356 "conf_lexer.l"
{ return MONTHS; }
	YY_BREAK
case 235:
YY_RULE_SETUP
#line 357 "conf_lexer.l"
{ return MONTHS; }
	YY_BREAK
case 236:
YY_RULE_SETUP
#line 358 "conf_lexer.l"
{ return WEEKS; }
	YY_BREAK
case 237:
YY_RULE_SETUP
#line 359 "conf_lexer.l"
{ return WEEKS; }
	YY_BREAK
case 238:
YY_RULE_SETUP
#line 360 "conf_lexer.l"
{ return DAYS; }
	YY_BREAK
case 239:
YY_RULE_SETUP
#line 361 "conf_lexer.l"
{ return DAYS; }
	YY_BREAK
case 240:
YY_RULE_SETUP
#line 362 "conf_lexer.l"
{ return HOURS; }
	YY_BREAK
case 241:
YY_RULE_SETUP
#line 363 "conf_lexer.l"
{ return HOURS; }
	YY_BREAK
case 242:
YY_RULE_SETUP
#line 364 "conf_lexer.l"
{ return MINUTES; }
	YY_BREAK
case 243:
YY_RULE_SETUP
#line 365 "conf_lexer.l"
{ return MINUTES; }
	YY_BREAK
case 244:
YY_RULE_SETUP
#line 366 "conf_lexer.l"
{ return SECONDS; }
	YY_BREAK
case 245:
YY_RULE_SETUP
#line 367 "conf_lexer.l"
{ return SECONDS; }
	YY_BREAK
case 246:
YY_RULE_SETUP
#line 369 "conf_lexer.l"
{ return BYTES; }
	YY_BREAK
case 247:
YY_RULE_SETUP
#line 370 "conf_lexer.l"
{ return BYTES; }
	YY_BREAK
case 248:
YY_RULE_SETUP
#line 371 "conf_lexer.l"
{ return KBYTES; }
	YY_BREAK
case 249:
YY_RULE_SETUP
#line 372 "conf_lexer.l"
{ return KBYTES; }
	YY_BREAK
case 250:
YY_RULE_SETUP
#line 373 "conf_lexer.l"
{ return KBYTES; }
	YY_BREAK
case 251:
YY_RULE_SETUP
#line 374 "conf_lexer.l"
{ return KBYTES; }
	YY_BREAK
case 252:
YY_RULE_SETUP
#line 375 "conf_lexer.l"
{ return KBYTES; }
	YY_BREAK
case 253:
YY_RULE_SETUP
#line 376 "conf_lexer.l"
{ return MBYTES; }
	YY_BREAK
case 254:
YY_RULE_SETUP
#line 377 "conf_lexer.l"
{ return MBYTES; }
	YY_BREAK
case 255:
YY_RULE_SETUP
#line 378 "conf_lexer.l"
{ return MBYTES; }
	YY_BREAK
case 256:
YY_RULE_SETUP
#line 379 "conf_lexer.l"
{ return MBYTES; }
	YY_BREAK
case 257:
YY_RULE_SETUP
#line 380 "conf_lexer.l"
{ return MBYTES; }
	YY_BREAK
case 258:
YY_RULE_SETUP
#line 381 "conf_lexer.l"
{ return TWODOTS; }
	YY_BREAK
case 259:
YY_RULE_SETUP
#line 383 "conf_lexer.l"
{ return yytext[0]; }
	YY_BREAK
case YY_STATE_EOF(INITIAL):
#line 384 "conf_lexer.l"
{ if (ieof()) yyterminate(); }
	YY_BREAK
case 260:
YY_RULE_SETUP
#line 386 "conf_lexer.l"
ECHO;
	YY_BREAK
#line 3435 "conf_lexer.c"

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
				yy_cp = (yy_last_accepting_cpos);
				yy_current_state = (yy_last_accepting_state);
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
	} /* end of user's declarations */
} /* end of yylex */
