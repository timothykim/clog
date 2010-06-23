CREATE TABLE IF NOT EXISTS entries
   (id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT,
    content TEXT,
    c_time INTEGER,
    u_time INTEGER,
	deleted INTEGER DEFAULT 0);

CREATE TABLE IF NOT EXISTS tags
   (id INTEGER PRIMARY KEY AUTOINCREMENT,
	name TEXT,
	count INTEGER);

CREATE TABLE IF NOT EXISTS comments 
   (id INTEGER PRIMARY KEY AUTOINCREMENT,
    entry_id INTEGER REFERENCES entries (id),
    c_time INTEGER,
    comment TEXT);

CREATE TABLE IF NOT EXISTS entry_tag
   (entry_id INTEGER REFERENCES entries (id),
	tag_id INTEGER REFERENCES tags (id));

CREATE VIEW entries_with_comment_count AS
    SELECT
        id, title, content, c_time, u_time,
        CASE WHEN comment_count THEN comment_count ELSE 0 END AS comment_count
    FROM
        entries
    LEFT OUTER JOIN
        (SELECT count(*) comment_count, entry_id
        FROM comments
        GROUP BY entry_id) AS ccount
    ON entries.id=ccount.entry_id
    WHERE entries.deleted = 0;
