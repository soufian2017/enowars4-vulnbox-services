from flask import Flask
from flask_cors import CORS

app = Flask(__name__)

# Set the secret key to some random bytes. Keep this really secret!
app.secret_key = 'FCY27Sk1UYHlrL1RY5TQAEKJBOJkApTbcmfc83m2b4ynhYzj6S'.encode()

CORS(app)

import routes
