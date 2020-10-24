static	void
info(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	if (!args || !*args)
	{
		say("This is ircII version %s", irc_version);
		say(" - originally written by Michael Sandrof");
		say(" - versions 2.1 to 2.2pre7 by Troy Rollo");
		say(" - development continued by matthew green");
		say("       e-mail: mrg@eterna.com.au  irc: phone");
		say(" - copyright (c) 1990-2004");
		say(" - do a /help ircii copyright for the full copyright");
		say(" - ircii includes software developed by the university");
		say("   of california, berkeley and its contributors");
		say("");
		say("ircii contributors");
		say("");
		say("\tMichael Sandrof       Mark T. Dameu");
		say("\tStellan Klebom        Carl v. Loesch");
		say("\tTroy Rollo            Martin  Friedrich");
		say("\tMichael Weber         Bill Wisner");
		say("\tRiccardo Facchetti    Stephen van den Berg");
		say("\tVolker Paulsen        Kare Pettersson");
		say("\tIan Frechette         Charles Hannum");
		say("\tmatthew green         christopher williams");
		say("\tJonathan Lemon        Brian Koehmstedt");
		say("\tNicolas Pioch         Brian Fehdrau");
		say("\tDarren Reed           Jeff Grills");
		say("\tJeremy Nelson         Philippe Levan");
		say("\tScott Reynolds        Glen McCready");
		say("\tChristopher Kalt      Joel Yliluoma");
		say("\tFlier");
	}
	if (get_server_version(from_server) != ServerICB)
		send_to_server("%s %s", command, args);
}
