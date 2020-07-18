from bonsai import LDAPClient, LDAPEntry
import random
import string
from ldap_function import register, instantiate_connection, register_vip
import time
from RandomUser import RandomUser

LDAPBASE_DN = "dc=example,dc=com"

_, conn = instantiate_connection()
# Check if already initialise
FILTER = '(ou=people)'
# Search for ou equals to people meaning already data there
response = conn.search(LDAPBASE_DN , 2, FILTER)
conn.close()

if len(response) == 0:
    print("Initialisation process")
    client, conn = instantiate_connection()
    # First delete users
    FILTER = '(&(!(cn=admin))(cn=*))'
    # Search for all user in the database which are not the admin user
    response = conn.search(LDAPBASE_DN , 2, FILTER)
    conn.close()

    # Delete found users
    for entry in response:
        _, conn = instantiate_connection()
        conn.delete(entry['dn'])
        conn.close()

    _, conn = instantiate_connection()
    # Then purge ou
    FILTER = '(ou=*)'
    # Search for all ou and erase them now
    response = conn.search(LDAPBASE_DN , 2, FILTER)
    conn.close()

    # Delete the ou
    for entry in response:
        _, conn = instantiate_connection()
        conn.delete(entry['dn'])
        conn.close()

    _, conn = instantiate_connection()
    # Add ou and user to LDAP server
    ou = LDAPEntry("ou=people,"+LDAPBASE_DN)
    ou['objectClass'] = ['organizationalUnit']
    conn.add(ou)
    conn.close()

    users = [RandomUser() for _ in range(20)]
    for user in users:
        register(user.get_username(), user.get_first_name(), user.get_last_name(), user.get_password())
        time.sleep(0.01)

    users = [RandomUser() for _ in range(20)]
    for user in users:
        register(user.get_username(), user.get_first_name(), user.get_last_name(), user.get_password(), department = "floor2")
        time.sleep(0.01)

    users = [RandomUser() for _ in range(200)]
    for user in users:
        register_vip(user.get_username(), user.get_first_name(), user.get_last_name(), user.get_password())
        time.sleep(0.01)

    users = [RandomUser() for _ in range(200)]
    for user in users:
        register_vip(user.get_username(), user.get_first_name(), user.get_last_name(), user.get_password(), department = "floor2")
        time.sleep(0.01)

    users = [RandomUser() for _ in range(5)]
    for user in users:
        register(user.get_username(), user.get_first_name(), user.get_last_name(), user.get_password(), department = "floor-secret")
        time.sleep(0.01)
