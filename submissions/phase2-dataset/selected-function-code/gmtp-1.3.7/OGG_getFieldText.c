gchar * OGG_getFieldText(const vorbis_comment *comments, const char *name) {
    gchar ** file_comments;
    gchar ** comments_split;
    gint file_comments_count = 0;
    // We simple cycle through our comments, looking for our name, and return it's value;

    if (comments->comments > 0) {
        file_comments = comments->user_comments;
        file_comments_count = comments->comments;
        while (file_comments_count--) {
            // We have our comment, now see if it is what we are after?
            comments_split = g_strsplit(*file_comments, "=", 2);
            if (*comments_split != NULL) {
                if (g_ascii_strcasecmp(name, *comments_split) == 0) {
                    // We have our desrired tag, so return it to the user.
                    comments_split++;
                    return g_strdup(*comments_split);
                }
            }
            // Increment our pointers accordingly.
            file_comments++;
        }
    } else {
        // No comments, so return a NULL value;
        return NULL;
    }
    // We didn't find our key, so return NULL
    return NULL;
}
