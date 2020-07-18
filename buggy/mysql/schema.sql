USE enodb;
CREATE TABLE users (
        id int NOT NULL AUTO_INCREMENT,
        name char(64),
        password char(64),
        status varchar(255),
        bonus int NOT NULL,
        admin boolean,
        PRIMARY KEY (id),
        INDEX (id),
        UNIQUE INDEX (name)
);
CREATE TABLE orders (
        id int NOT NULL AUTO_INCREMENT,
        name char(64),
        itemID int,
        color char(64),
        quantity int,
        hash char(64),
        PRIMARY KEY (id),
        INDEX (id),
        INDEX (name),
        INDEX (hash)
);
CREATE TABLE messages (
        name char(64),
        sender char(64),
        hash char(64),
        message varchar(512),
        INDEX (hash),
        INDEX (name)
);
CREATE TABLE comments (
        id int NOT NULL AUTO_INCREMENT,
        created_at timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
        name char(64),
        product char(64),
        content varchar(512),
        PRIMARY KEY (id),
        INDEX (id),
        INDEX (product)
);
CREATE TABLE tickets (
        name char(64),
        subject char(64),
        hash char(64),
        PRIMARY KEY (hash),
        INDEX (hash),
        INDEX (name)
);
INSERT INTO enodb.users (name, password, status, bonus, admin)
VALUES("admin", "root", "", 0, true);
DELIMITER |
CREATE EVENT ttl_delete
  ON SCHEDULE EVERY 300 SECOND DO BEGIN
    DELETE FROM enodb.comments
    WHERE created_at < NOW() - INTERVAL 1800 SECOND;
  END |
DELIMITER ;
