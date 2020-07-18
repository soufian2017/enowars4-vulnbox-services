from flask import Flask, request, jsonify, render_template, redirect, url_for, session
from ldap_function import search, authenticate, register, getUser, register_vip
from app import app

import flask_jwt as jwt
from flask_api import status
import json
from crypto import verifyUsingKey
from base64 import b64decode
from RandomUser import randomString
## Loading of public key
from cryptography.hazmat.primitives.serialization import load_pem_public_key
from cryptography.hazmat.backends import default_backend


SYSTEM_PUB_KEY = open('./SYS_KEY.pem').read()


############ Angular specific ############
@app.route('/ang/search', methods=['GET'])
def get_search_results_angular():

    search_attribute = request.args.get('attribute')
    search_query = request.args.get('query')
    result = search(search_attribute, search_query)

    return jsonify(results= result)

@app.route('/ang/login', methods=['POST'])
def post_login_angular():
    usr = request.form.get("user")
    pwd = request.form.get("pass")

    #This returns True if user can be authenticated, false otherwise
    authenticated = authenticate(usr, pwd)

    if(authenticated):
        userDict = getUser(usr)

        if userDict is None:
            return 'Fail to fetch user', status.HTTP_400_BAD_REQUEST
        else:
            return jsonify(token = userDict)
    else:
        return 'wrongpass', status.HTTP_401_UNAUTHORIZED

@app.route('/ang/register', methods=['POST'])
def post_register_angular():
    username = request.form.get("user")
    password = request.form.get("pass")
    firstname = request.form.get('firstname')
    surname = request.form.get('surname')
    department = request.form.get('department')
    address = request.form.get('address')
    phone = request.form.get('phoneNumber')

    privKey = register(username, firstname, surname, password, department, address, phone)

    if privKey is None :
        return 'Failed registration', status.HTTP_400_BAD_REQUEST
    else :
        return jsonify(pk=str(privKey))

@app.route('/ang/registerVIP', methods=['GET', 'POST'])
def VIPregister_angular():
    if request.method == 'POST':
        username = request.form.get("user")
        password = request.form.get("pass")
        firstname = request.form.get('firstname')
        surname = request.form.get('surname')
        department = request.form.get('department')
        address = request.form.get('address')
        phone = request.form.get('phoneNumber')
        challengeResponse = request.form.get('challengeResponse')

        if 'challenge' in session:
            challenge = session['challenge']
            publicKey = load_pem_public_key(str.encode(SYSTEM_PUB_KEY), backend=default_backend())
            challengeResponseDecoded = verifyUsingKey(publicKey, b64decode(challengeResponse), challenge.encode())
            if challengeResponseDecoded:
                privKey = register_vip(username, firstname, surname, password, department, address, phone)

                if privKey is None :
                    return 'Failed registration', status.HTTP_400_BAD_REQUEST
                else :
                    return jsonify(pk=str(privKey))
            else:
                return 'Failed registration', status.HTTP_403_FORBIDDEN
        else:
            return 'Failed registration', status.HTTP_403_FORBIDDEN
    else:
        challenge = randomString(25)
        session['challenge'] = challenge
        return challenge
