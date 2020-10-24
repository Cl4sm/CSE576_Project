static void report_format_hints(void){
  @ <hr><h3>TICKET Schema</h3>
  @ <blockquote><pre>
  @ CREATE TABLE ticket(
  @    tn integer primary key,  -- Unique tracking number for the ticket
  @    type text,               -- code, doc, todo, new, or event
  @    status text,             -- new, review, defer, active, fixed,
  @                             -- tested, or closed
  @    origtime int,            -- Time this ticket was first created
  @    changetime int,          -- Time of most recent change to this ticket
  @    derivedfrom int,         -- This ticket derived from another
  @    version text,            -- Version or build number
  @    assignedto text,         -- Whose job is it to deal with this ticket
  @    severity int,            -- How bad is the problem
  @    priority text,           -- When should the problem be fixed
  @    subsystem text,          -- What subsystem does this ticket refer to
  @    owner text,              -- Who originally wrote this ticket
  @    title text,              -- Title of this bug
  @    description text,        -- Description of the problem
  @    remarks text             -- How the problem was dealt with
  @ );
  @ </pre></blockquote>
  @ <h3>Notes</h3>
  @ <ul>
  @ <li><p>The SQL must consist of a single SELECT statement</p></li>
  @
  @ <li><p>If a column of the result set is named "#" then that column
  @ is assumed to hold a ticket number.  A hyperlink will be created from
  @ that column to a detailed view of the ticket.</p></li>
  @
  @ <li><p>If a column of the result set is named "bgcolor" then the content
  @ of that column determines the background color of the row.</p></li>
  @
  @ <li><p>Times in the TICKET table are expressed as seconds since 1970.
  @ Convert these values to human-friendly date formats using the
  @ <b>sdate()</b> and <b>ldate()</b> SQL functions.</p></li>
  @ 
  @ <li><p>The <b>now()</b> SQL function returns the current time and date
  @ in seconds since 1970.  The <b>user()</b> SQL function returns a string
  @ which is the login of the current user.</p></li>
  @
  @ <li><p>The first column whose name begins with underscore ("_") and all
  @ subsequent columns are shown on their own rows in the table.  This is
  @ useful for displaying the TICKET.DESCRIPTION and TICKET.REMARKS fields.
  @ </p></li>
  @
  @ <li><p>The <b>aux()</b> SQL function takes a parameter name as an argument
  @ and returns the value that the user enters in the resulting HTML form. A
  @ second optional parameter provides a default value for the field.</p></li>
  @
  @ <li><p>The <b>option()</b> SQL function takes a parameter name
  @ and a quoted SELECT statement as parameters. The query results are
  @ presented as an HTML dropdown menu and the function returns
  @ the currently selected value. Results may be a single value column or
  @ two <b>value,description</b> columns. The first row is the default.</p></li>
  @
  @ <li><p>The <b>cgi()</b> SQL function takes a parameter name as an argument
  @ and returns the value of a corresponding CGI query value. If the CGI
  @ parameter doesn't exist, an optional second argument will be returned
  @ instead.</p></li>
  @
  @ <li><p>If a column is wrapped by the <b>wiki()</b> SQL function, it will
  @ be rendered as wiki formatted content.</p></li>
  @
  @ <li><p>If a column is wrapped by the <b>tkt()</b> SQL function, it will
  @ be shown as a ticket number with a link to the appropriate page</p></li>
  @
  @ <li><p>If a column is wrapped by the <b>chng()</b> SQL function, it will
  @ be shown as a checkin number with a link to the appropriate page.</p></li>
  @
  @ <li><p>The <b>path()</b> SQL function can be used to extract complete filename
  @ from FILE table. For example:
  @ <pre>SELECT path(isdir, dir, base) AS 'filename' FROM file</pre>
  @ </p></li>
  @
  @ <li><p>The <b>dirname()</b> SQL function takes filename as only argument
  @ and extracts parent directory name from it.</p></li>
  @
  @ <li><p>The <b>basename()</b> SQL function takes filename as only argument
  @ and extracts basename from it.</p></li>
  @
  @ <li><p>The <b>search()</b> SQL function takes a keyword pattern and
  @ a search text. The function returns an integer score which is
  @ higher depending on how well the search went.</p></li>
  @
  @ <li><p>The query can join other tables in the database besides TICKET.
  @ </p></li>
  @ </ul>
  @
  @ <h3>Examples</h3>
  @ <p>In this example, the first column in the result set is named
  @ "bgcolor".  The value of this column is not displayed.  Instead, it
  @ selects the background color of each row based on the TICKET.STATUS
  @ field of the database.  The color key at the right shows the various
  @ color codes.</p>
  @ <table align="right" hspace=5 border=1 cellspacing=0 width=125>
  @ <tr bgcolor="#f2dcdc"><td align="center">new or active</td></tr>
  @ <tr bgcolor="#e8e8bd"><td align="center">review</td></tr>
  @ <tr bgcolor="#cfe8bd"><td align="center">fixed</td></tr>
  @ <tr bgcolor="#bde5d6"><td align="center">tested</td></tr>
  @ <tr bgcolor="#cacae5"><td align="center">defer</td></tr>
  @ <tr bgcolor="#c8c8c8"><td align="center">closed</td></tr>
  @ </table>
  @ <blockquote><pre>
  @ SELECT
  @   CASE WHEN status IN ('new','active') THEN '#f2dcdc'
  @        WHEN status='review' THEN '#e8e8bd'
  @        WHEN status='fixed' THEN '#cfe8bd'
  @        WHEN status='tested' THEN '#bde5d6'
  @        WHEN status='defer' THEN '#cacae5'
  @        ELSE '#c8c8c8' END as 'bgcolor',
  @   tn AS '#',
  @   type AS 'Type',
  @   status AS 'Status',
  @   sdate(origtime) AS 'Created',
  @   owner AS 'By',
  @   subsystem AS 'Subsys',
  @   sdate(changetime) AS 'Changed',
  @   assignedto AS 'Assigned',
  @   severity AS 'Svr',
  @   priority AS 'Pri',
  @   title AS 'Title'
  @ FROM ticket
  @ </pre></blockquote>
  @ <p>To base the background color on the TICKET.PRIORITY or
  @ TICKET.SEVERITY fields, substitute the following code for the
  @ first column of the query:</p>
  @ <table align="right" hspace=5 border=1 cellspacing=0 width=125>
  @ <tr bgcolor="#f2dcdc"><td align="center">1</td></tr>
  @ <tr bgcolor="#e8e8bd"><td align="center">2</td></tr>
  @ <tr bgcolor="#cfe8bd"><td align="center">3</td></tr>
  @ <tr bgcolor="#cacae5"><td align="center">4</td></tr>
  @ <tr bgcolor="#c8c8c8"><td align="center">5</td></tr>
  @ </table>
  @ <blockquote><pre>
  @ SELECT
  @   CASE priority WHEN 1 THEN '#f2dcdc'
  @        WHEN 2 THEN '#e8e8bd'
  @        WHEN 3 THEN '#cfe8bd'
  @        WHEN 4 THEN '#cacae5'
  @        ELSE '#c8c8c8' END as 'bgcolor',
  @ ...
  @ FROM ticket
  @ </pre></blockquote>
#if 0
  @ <p>You can, of course, substitute different colors if you choose.
  @ Here is a palette of suggested background colors:</p>
  @ <blockquote>
  @ <table border=1 cellspacing=0 width=300>
  @ <tr><td align="center" bgcolor="#ffbdbd">#ffbdbd</td>
  @     <td align="center" bgcolor="#f2dcdc">#f2dcdc</td></tr>
  @ <tr><td align="center" bgcolor="#ffffbd">#ffffbd</td>
  @     <td align="center" bgcolor="#e8e8bd">#e8e8bd</td></tr>
  @ <tr><td align="center" bgcolor="#c0ebc0">#c0ebc0</td>
  @     <td align="center" bgcolor="#cfe8bd">#cfe8bd</td></tr>
  @ <tr><td align="center" bgcolor="#c0c0f4">#c0c0f4</td>
  @     <td align="center" bgcolor="#d6d6e8">#d6d6e8</td></tr>
  @ <tr><td align="center" bgcolor="#d0b1ff">#d0b1ff</td>
  @     <td align="center" bgcolor="#d2c0db">#d2c0db</td></tr>
  @ <tr><td align="center" bgcolor="#bbbbbb">#bbbbbb</td>
  @     <td align="center" bgcolor="#d0d0d0">#d0d0d0</td></tr>
  @ </table>
  @ </blockquote>
#endif
  @ <p>To see the TICKET.DESCRIPTION and TICKET.REMARKS fields, include
  @ them as the last two columns of the result set and given them names
  @ that begin with an underscore.  Like this:</p>
  @ <blockquote><pre>
  @  SELECT
  @    tn AS '#',
  @    type AS 'Type',
  @    status AS 'Status',
  @    sdate(origtime) AS 'Created',
  @    owner AS 'By',
  @    subsystem AS 'Subsys',
  @    sdate(changetime) AS 'Changed',
  @    assignedto AS 'Assigned',
  @    severity AS 'Svr',
  @    priority AS 'Pri',
  @    title AS 'Title',
  @    description AS '_Description',   -- When the column name begins with '_'
  @    remarks AS '_Remarks'            -- the data is shown on a separate row.
  @  FROM ticket
  @ </pre></blockquote>
  @
  @ <p>Or, to see part of the description on the same row, use the
  @ <b>wiki()</b> function with some string manipulation. Using the
  @ <b>tkt()</b> function on the ticket number will also generate a linked
  @ field, but without the extra <i>edit</i> column:
  @ </p>
  @ <blockquote><pre>
  @  SELECT
  @    tkt(tn) AS '',
  @    title AS 'Title',
  @    wiki(substr(description,0,80)) AS 'Description'
  @  FROM ticket
  @ </pre></blockquote>
  @
}