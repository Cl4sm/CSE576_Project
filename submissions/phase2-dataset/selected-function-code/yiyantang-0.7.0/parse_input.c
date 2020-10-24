void parse_input(char* ibuf, int* ret, yytconfig *curconfig, keystate *inputstate)
{
	int i;

	for(i=0; i < *ret; i++)
	{
		/* We don't want to screw alt-ctrl-key. */
		if(ibuf[i] == ALTKEY && !inputstate->cmdstate) 
		{
			inputstate->altstate = 1;
		}
		else 
		{
			if(!inputstate->altstate)
			{
				if(ibuf[i] == curconfig->yytesc)
				{
					inputstate->cmdstate = 1;
					shiftbuf(&ibuf[i], ret, &i);
					/* ibuf[i] = BEEP; */
				} /* YYTESC */

				else if(inputstate->cmdstate)
				{
					switch(ibuf[i])
					{
						case 'g':
							curconfig->fromcode = GB_CODE;
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
						case 'b':
							curconfig->fromcode = BIG5_CODE;
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
						case 'h':
							curconfig->fromcode = HZ_CODE;
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
						case 'u':
							curconfig->fromcode = UNI_CODE;
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
						case '7':
							curconfig->fromcode = UTF7_CODE;
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
						case '8':
							curconfig->fromcode = UTF8_CODE;
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
						case 'd':	/* Disable conversion. */
							curconfig->fromcode = -2;
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
						case 'a':	/* autoconversion. */
							curconfig->fromcode = 0;
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
						case 'r':	/* reset the initial state. */
							curconfig->fromcode = -1;
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
						case 'y':	/* write the escape key. */
							ibuf[i] = curconfig->yytesc;
							break;
						default:
							shiftbuf(&ibuf[i], ret, &i);
							/* ibuf[i] = BEEP; */
							break;
					}	/* switch */
					inputstate->cmdstate = 0; /* Reset the state. */
				} /* else if cmdstate. */

			} /* if !altstate. */
			inputstate->altstate = 0;
		} /* else ALTKEY. */
	} /* for loop. */
}
