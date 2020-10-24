void
mcd_account_altered_by_plugin (McdAccount *account,
                               const gchar *name)
{
    /* parameters are handled en bloc, reinvoke self with bloc key: */
    if (g_str_has_prefix (name, "param-"))
    {
        mcd_account_altered_by_plugin (account, "Parameters");
    }
    else
    {
        guint i = 0;
        const McdDBusProp *prop = NULL;
        GValue value = G_VALUE_INIT;
        GError *error = NULL;

        DEBUG ("%s", name);

        if (tp_strdiff (name, "Parameters") &&
            !mcd_storage_init_value_for_attribute (&value, name))
        {
            WARNING ("plugin wants to alter %s but I don't know what "
                     "type that ought to be", name);
            return;
        }

        if (!tp_strdiff (name, "Parameters"))
        {
            get_parameters (TP_SVC_DBUS_PROPERTIES (account), name, &value);
        }
        else if (!mcd_storage_get_attribute (account->priv->storage,
                                             account->priv->unique_name,
                                             name, &value, &error))
        {
            WARNING ("cannot get new value of %s: %s", name, error->message);
            g_error_free (error);
            return;
        }

        /* find the property update handler */
        for (; prop == NULL && account_properties[i].name != NULL; i++)
        {
            if (g_str_equal (name, account_properties[i].name))
                prop = &account_properties[i];
        }

        /* is a known property: invoke the getter method for it (if any): *
         * then issue the change notification (DBus signals etc) for it   */
        if (prop != NULL)
        {
            /* poke the value back into itself with the setter: this      *
             * extra round-trip may trigger extra actions like notifying  *
             * the connection manager of the change, even though our own  *
             * internal storage already has this value and needn't change */
            if (prop->setprop != NULL)
            {
                DEBUG ("Calling property setter for %s", name);
                if (!prop->setprop (TP_SVC_DBUS_PROPERTIES (account),
                                    prop->name, &value,
                                    MCD_DBUS_PROP_SET_FLAG_ALREADY_IN_STORAGE,
                                    &error))
                {
                    WARNING ("Unable to set %s: %s", name, error->message);
                    g_error_free (error);
                }
            }
            else
            {
                DEBUG ("Emitting signal directly for %s", name);
                mcd_account_changed_property (account, prop->name, &value);
            }
        }
        else
        {
            DEBUG ("%s does not appear to be an Account property", name);
        }

        g_value_unset (&value);
    }
}