set_var_value(var_index, value)
	int	var_index;
	u_char	*value;
{
	u_char	*rest;
	IrcVariable *var;
	int	old;


	var = &(irc_variable[var_index]);
#ifdef DAEMON_UID
	if (getuid() == DAEMON_UID && var->flags&VF_NODAEMON && value && *value)
	{
		say("You are not permitted to set that variable");
		return;
	}
#endif /* DAEMON_UID */
	switch (var->type)
	{
	case BOOL_TYPE_VAR:
		if (value && *value && (value = next_arg(value, &rest)))
		{
			old = var->integer;
			if (do_boolean(value, &(var->integer)))

			{
				say("Value must be either ON, OFF, or TOGGLE");
				break;
			}
			if (!(var->int_flags & VIF_CHANGED))
			{
				if (old != var->integer)
					var->int_flags |= VIF_CHANGED;
			}
			if (loading_global)
				var->int_flags |= VIF_GLOBAL;
			if (var->ifunc)
				(*var->ifunc)(var->integer);
			say("Value of %s set to %s", var->name,
				var->integer ? var_settings[ON]
					     : var_settings[OFF]);
		}
		else
			say("Current value of %s is %s", var->name,
				(var->integer) ?
				var_settings[ON] : var_settings[OFF]);
		break;
	case CHAR_TYPE_VAR:
		if (value && *value && (value = next_arg(value, &rest)))
		{
			if ((int) my_strlen(value) > 1)
				say("Value of %s must be a single character",
					var->name);
			else
			{
				if (!(var->int_flags & VIF_CHANGED))
				{
					if (var->integer != *value)
						var->int_flags |= VIF_CHANGED;
				}
				if (loading_global)
					var->int_flags |= VIF_GLOBAL;
				var->integer = *value;
				if (var->ifunc)
					(*var->ifunc)(var->integer);
				say("Value of %s set to '%c'", var->name,
					var->integer);
			}
		}
		else
			say("Current value of %s is '%c'", var->name,
				var->integer);
		break;
	case INT_TYPE_VAR:
		if (value && *value && (value = next_arg(value, &rest)))
		{
			int	val;

			if (!is_number(value))
			{
				say("Value of %s must be numeric!", var->name);
				break;
			}
			if ((val = my_atoi(value)) < 0)
			{
				say("Value of %s must be greater than 0",
					var->name);
				break;
			}
			if (!(var->int_flags & VIF_CHANGED))
			{
				if (var->integer != val)
					var->int_flags |= VIF_CHANGED;
			}
			if (loading_global)
				var->int_flags |= VIF_GLOBAL;
			var->integer = val;
			if (var->ifunc)
				(*var->ifunc)(var->integer);
			say("Value of %s set to %d", var->name, var->integer);
		}
		else
			say("Current value of %s is %d", var->name,
				var->integer);
		break;
	case STR_TYPE_VAR:
		if (value)
		{
			if (*value)
			{
				u_char	*temp = NULL;

				if (var->flags & VF_EXPAND_PATH)
				{
					temp = expand_twiddle(value);
					if (temp)
						value = temp;
					else
						say("SET: no such user");
				}
				if ((!var->int_flags & VIF_CHANGED))
				{
					if ((var->string && ! value) ||
					    (! var->string && value) ||
					    my_stricmp(var->string, value))
						var->int_flags |= VIF_CHANGED;
				}
				if (loading_global)
					var->int_flags |= VIF_GLOBAL;
				malloc_strcpy(&var->string, value);
				if (temp)
					new_free(&temp);
			}
			else
			{
				if (var->string)
					say("Current value of %s is %s",
						var->name, var->string);
				else
					say("No value for %s has been set",
						var->name);
				return;
			}
		}
		else
			new_free(&var->string);
		if (var->sfunc)
			(*var->sfunc)(var->string);
		say("Value of %s set to %s", var->name, var->string ?
			var->string : UP("<EMPTY>"));
		break;
	}
}
