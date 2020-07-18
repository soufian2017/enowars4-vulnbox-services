#!/bin/sh
mkdir -p /data/cookies /data/priority_towels /data/towels /data/users;
chown -R poolcide:poolcide /data;
su poolcide -c "python3 poolcgid.py";
