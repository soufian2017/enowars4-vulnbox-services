import math
import random
from cryptography.hazmat.primitives.serialization import load_pem_public_key, load_pem_private_key
from cryptography.hazmat.primitives.asymmetric import rsa, padding, utils
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
from cryptography.exceptions import InvalidSignature
import hashlib, binascii, os


def hash_password(password):
    """Hash a password for storing."""
    salt = hashlib.sha256(os.urandom(60)).hexdigest().encode('utf-8')
    pwdhash = hashlib.pbkdf2_hmac('sha512', password.encode('utf-8'),
                                salt, 100)
    pwdhash = binascii.hexlify(pwdhash)
    return (salt + pwdhash).decode('utf-8')


def verify_password(stored_password, provided_password):
    """Verify a stored password against one provided by user"""
    salt = stored_password[:64]
    stored_password = stored_password[64:]
    pwdhash = hashlib.pbkdf2_hmac('sha512',
                                  provided_password.encode('utf-8'),
                                  salt.encode('utf-8'),
                                  100)
    pwdhash = binascii.hexlify(pwdhash).decode('utf-8')
    return pwdhash == stored_password


def generateKeyPair():
    key_dir = '/keys/'
    if not os.path.exists(key_dir):
        os.mkdir(key_dir)
    try:
        key_number = random.randint(0,2000-1)
        keyfile = f'{key_dir}privkey_{key_number}.pem'
        privatekey = load_pem_private_key(str.encode(open(keyfile).read()), password=None, backend=default_backend())
        publickey = load_pem_public_key(str.encode(open(keyfile.replace('privkey', 'pubkey')).read()), backend=default_backend())
        return (privatekey, publickey)
    except:
        raise Exception("Problem with keys")
        return None


def encodeUsingKey(public_key, toEncode):
    ciphertext = public_key.encrypt(
        toEncode,
        padding.OAEP(
            mgf=padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )
    return ciphertext

def verifyUsingKey(public_key, signature, data):
    try:
        public_key.verify(
            signature,
            data,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )
        return True
    except InvalidSignature:
        return False
