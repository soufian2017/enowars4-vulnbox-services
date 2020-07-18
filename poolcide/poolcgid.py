#!/usr/bin/env python3
from http.server import ThreadingHTTPServer, CGIHTTPRequestHandler

PORT = 9001

if __name__ == "__main__":
    print(f"serving on http://127.0.0.1:{PORT}")
    ThreadingHTTPServer(("0.0.0.0", PORT), CGIHTTPRequestHandler).serve_forever()
