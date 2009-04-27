CREATE TABLE IF NOT EXISTS entries
   (id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT,
    content TEXT,
    c_time INTEGER,
    u_time INTEGER,
	deleted INTEGER DEFAULT 0,
    comment_count INTEGER DEFAULT 0);

CREATE TABLE IF NOT EXISTS tags
   (id INTEGER PRIMARY KEY AUTOINCREMENT,
	name TEXT,
	count INTEGER);

CREATE TABLE IF NOT EXISTS commnets
   (id INTEGER PRIMARY KEY AUTOINCREMENT,
    entry_id INTEGER REFERENCES entries (id),
    c_time INTEGER,
    comment TEXT);

CREATE TABLE IF NOT EXISTS entry_tag
   (entry_id INTEGER REFERENCES entries (id),
	tag_id INTEGER REFERENCES tags (id));


  
