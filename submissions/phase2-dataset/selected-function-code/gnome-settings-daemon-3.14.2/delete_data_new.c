DeleteData *
delete_data_new (GFile        *file,
                 GCancellable *cancellable,
                 GDateTime    *old,
                 gboolean      dry_run,
                 gboolean      trash,
                 gint          depth)
{
        DeleteData *data;

        data = g_new (DeleteData, 1);
        data->ref_count = 1;
        data->file = g_object_ref (file);
        data->cancellable = cancellable ? g_object_ref (cancellable) : NULL;
        data->old = g_date_time_ref (old);
        data->dry_run = dry_run;
        data->trash = trash;
        data->depth = depth;
        data->name = g_file_get_parse_name (data->file);

        return data;
}