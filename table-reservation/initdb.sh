#!/bin/bash
set -e

#psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
#    CREATE USER docker;
#    CREATE DATABASE docker;
#    GRANT ALL PRIVILIGES ON DATABASE docker TO docker
#EOSQL

psql -v ON_ERROR_STOP=1 -A -p 5432 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    CREATE TABLE IF NOT EXISTS reservations (
            id text PRIMARY KEY,
            name text NOT NULL,
            personCount integer,
            tableID integer,
            meal text,
            time text,
            timestamp TIMESTAMPTZ NOT NULL DEFAULT NOW()
            );
EOSQL
psql -v ON_ERROR_STOP=1 -A -p 5432 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    CREATE TABLE IF NOT EXISTS tables (
                tableID serial8 PRIMARY KEY,
                seats integer
                );
EOSQL
psql -v ON_ERROR_STOP=1 -A -p 5432 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    CREATE TABLE IF NOT EXISTS owner (
                firstname text PRIMARY KEY,
                lastname text,
                age integer
                );
EOSQL
psql -v ON_ERROR_STOP=1 -A -p 5432 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    CREATE TABLE IF NOT EXISTS meals (
                id serial8 PRIMARY KEY,
                name text,
                price money
                );
EOSQL
psql -v ON_ERROR_STOP=1 -A -p 5432 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    INSERT INTO reservations VALUES ('\0x000', 'ENO-FLAG ;)', 1, 1, 1, '2:32');
EOSQL
psql -v ON_ERROR_STOP=1 -A -p 5432 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    INSERT INTO meals VALUES (1, 'Caribbean Fish Stew', 14.31);
    INSERT INTO meals VALUES (2, 'Pineapple Pork Chops', 16.96);
    INSERT INTO meals VALUES (3, 'Cuban Sandwich', 10.93);
    INSERT INTO meals VALUES (4, 'Caribbean Crab Cakes Benedict', 15.86);
    INSERT INTO meals VALUES (5, 'Mixed Grill Kebabs with Guava BBQ Sauce', 19.78);
    INSERT INTO meals VALUES (6, 'Tropical Fruit, Avocado and Grilled Shrimp Salad', 12.19);
    INSERT INTO meals VALUES (7, 'Scallops with Mango Vinaigrette', 22.85);
    INSERT INTO meals VALUES (8, 'Jerk Chicken with Coconut Rice', 17.45);
    INSERT INTO meals VALUES (9, 'Chicken Kebabs with Lime-Cayenne Butter', 17.70);
    INSERT INTO meals VALUES (10, 'Caribbean-spiced Shrimp with Pineapple Salsa', 18.22);
EOSQL
psql -v ON_ERROR_STOP=1 -A -p 5432 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    INSERT INTO tables VALUES (1, 4);
    INSERT INTO tables VALUES (2, 4);
    INSERT INTO tables VALUES (3, 4);
    INSERT INTO tables VALUES (4, 12);
    INSERT INTO tables VALUES (5, 4);
    INSERT INTO tables VALUES (6, 4);
    INSERT INTO tables VALUES (7, 12);
    INSERT INTO tables VALUES (8, 12);
    INSERT INTO tables VALUES (9, 10);
    INSERT INTO tables VALUES (10, 2);
    INSERT INTO tables VALUES (11, 6);
    INSERT INTO tables VALUES (12, 6);
    INSERT INTO tables VALUES (13, 8);
    INSERT INTO tables VALUES (14, 2);
    INSERT INTO tables VALUES (15, 10);
    INSERT INTO tables VALUES (16, 12);
    INSERT INTO tables VALUES (17, 2);
    INSERT INTO tables VALUES (18, 6);
    INSERT INTO tables VALUES (19, 6);
    INSERT INTO tables VALUES (20, 4);
EOSQL
psql -v ON_ERROR_STOP=1 -A -p 5432 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    INSERT INTO owner VALUES ('Jerome', 'McElroy', 51);
EOSQL
