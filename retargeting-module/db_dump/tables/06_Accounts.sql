CREATE TABLE IF NOT EXISTS Accounts
(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name VARCHAR(2048),
	UNIQUE (name) ON CONFLICT IGNORE
);
INSERT INTO Accounts(name) VALUES('');