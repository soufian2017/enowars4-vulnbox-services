########## initialize connection ###############################################
import sys
from bonsai import LDAPClient, LDAPEntry
from crypto import generateKeyPair, encodeUsingKey, verify_password, hash_password
from cryptography.hazmat.primitives import serialization

LDAPURI = "ldap://ldapserver"
LDAPBASE_DN = "dc=example,dc=com"
PEOPLE_DN = "ou=people,"+LDAPBASE_DN
ADMIN_DN = "cn=admin,"+LDAPBASE_DN
BINDPW = "admin"
departmentList = ['floor1', 'floor2', 'floor-secret']
VipDepartmentList = ['floor1', 'floor2']

def instantiate_connection(DN = ADMIN_DN, PASSWD = BINDPW):
    client = LDAPClient(LDAPURI)
    client.set_credentials("SIMPLE", user=DN, password=PASSWD)
    conn = client.connect()
    return (client, conn)


def authenticate(username, password):
    # Bind read only user and search for user
    client, conn = instantiate_connection()
    FILTER = f'(cn={username})'
    # Search for user existence
    response = conn.search(LDAPBASE_DN , 2, FILTER, ['userPassword'])
    conn.close()
    nb_results = len(response)

    if nb_results != 1:
        return False
    else:
        user_pass = response[0]['userPassword'][0]
        return verify_password(user_pass, password)

def getUser(username):
    # Instantiate connection
    client, conn = instantiate_connection()

    # Check if user already exists
    FILTER = f'(cn={sanitize(username)})'
    attributes = ['givenName',
                    'sn',
                    'homePostalAddress',
                    'departmentNumber',
                    'telephoneNumber',
                    'sshPublicKey']
    response = conn.search(LDAPBASE_DN , 2, FILTER, attributes)
    conn.close()

    nb_results = len(response)
    if nb_results > 1:
        return None
    else:
        dict = response[0]
        user_dict = {}
        for key, value in dict.items(exclude_dn=True):
            try:
                user_dict.update({str(key): str(value[0])})
            except:
                user_dict.update({str(key): str(value)})
        return user_dict

def register_vip(username, firstname, surname, password, department = "floor1", address="Nowhere", phone=102):
    if len(firstname) > 24 or len(surname) > 24:
        return None
    if department not in VipDepartmentList:
        return None

    # Instantiate connection
    client, conn = instantiate_connection()

    # Check if user already exists
    FILTER = f'(cn={sanitize(username)})'
    response = conn.search(LDAPBASE_DN , 2, FILTER)
    conn.close()
    nb_results = len(response)
    if nb_results >= 1:
        return None

    else:
        # Generate Key Pair
        (private_key, public_key) = generateKeyPair()
        enc_address = encodeUsingKey(public_key, str.encode(address)).hex()

        hash_pass = hash_password(password)

        # Connect using admin account
        server, conn = instantiate_connection()
        # Build user DN
        USER_DN = f'cn={username},ou=people,{LDAPBASE_DN}'

        # Build user entry
        user = LDAPEntry(USER_DN)
        user['objectClass'] = ['person', 'inetOrgPerson', 'ldapPublicKey']
        user['givenName'] = firstname
        user['sn'] = surname
        user['homePostalAddress'] = enc_address
        user['telephoneNumber'] = int(phone)
        user['departmentNumber'] = department
        user['userPassword'] = hash_pass
        # Handle intern keys and non intern keys
        user['sshPublicKey'] = public_key.public_bytes(
                                encoding=serialization.Encoding.PEM,
                                format=serialization.PublicFormat.SubjectPublicKeyInfo)

        print_priv = private_key.private_bytes(
                        encoding=serialization.Encoding.PEM,
                        format=serialization.PrivateFormat.TraditionalOpenSSL,
                        encryption_algorithm=serialization.NoEncryption())

        add_sucess = conn.add(user)
        conn.close()
        if (add_sucess):
            return print_priv
        else:
            return None


def register(username, firstname, surname, password, department = "floor1", address="Nowhere", phone=102):

    # Register checks, still some to implement
    if len(firstname) > 24 or len(surname) > 24:
        return None
    if department not in departmentList:
        return None

    # Instantiate connection
    client, conn = instantiate_connection()

    # Check if user already exists
    FILTER = f'(cn={sanitize(username)})'
    response = conn.search(LDAPBASE_DN , 2, FILTER)
    conn.close()
    nb_results = len(response)
    if nb_results >= 1:
        return None

    else:
        hash_pass = hash_password(password)

        # Connect using admin account
        server, conn = instantiate_connection()
        # Build user DN
        USER_DN = f'cn={username},ou=people,{LDAPBASE_DN}'

        # Build user entry
        user = LDAPEntry(USER_DN)
        user['objectClass'] = ['person', 'inetOrgPerson', 'ldapPublicKey']
        user['givenName'] = firstname
        user['sn'] = surname
        # Non VIP user doesn't enjoy encryption for data
        user['homePostalAddress'] = address
        # Telephone number
        try :
            user['telephoneNumber'] = int(phone)
        except:
            user['telephoneNumber'] = 123456

        user['departmentNumber'] = department
        user['userPassword'] = hash_pass
        # Handle intern keys and non intern keys
        user['sshPublicKey'] = b'No key for non VIP user'
        print_priv = b'No key for non VIP user'

        add_sucess = conn.add(user)
        conn.close()
        if (add_sucess):
            return print_priv
        else:
            return None

def search(attribute, query):
    server, conn = instantiate_connection()

    FILTER = f'(|(&({sanitize(attribute)}=*{sanitize(query)}*)(departmentNumber=floor1))(&({sanitize(attribute)}=*{sanitize(query)}*)(departmentNumber=floor2)))'

    attributes = ['cn', 'givenName', 'sn', 'homePostalAddress', 'sshPublicKey', attribute]
    response = conn.search(LDAPBASE_DN , 2, FILTER, attributes)
    conn.close()

    nb_results = len(response)
    responses = []
    for entry in response:
        values = []
        for value in entry.values(exclude_dn=True):
            values.append(str(value[0]))
        responses.append(values)
    return responses

def sanitize(string):
    return string.replace("*", "")
