#!/bin/bash

sleep 5

# Start Gunicorn processes
echo "init LDAP "
python3 init_script.py &

echo "Starting Gunicorn."
exec gunicorn -c gunicorn.conf.py wsgi:app
