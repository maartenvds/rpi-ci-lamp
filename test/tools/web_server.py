from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
import ssl

FILE_PREFIX = "test/data/"

class RequestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type','text/html')
        self.end_headers()
        # Send the html message
        branch = self.path.split('/')[-1]
        print branch
        with open(FILE_PREFIX + branch, "r") as f:
            self.wfile.write(f.read())

try:
    server = HTTPServer(('localhost', 4443), RequestHandler)
    server.socket = ssl.wrap_socket(server.socket, certfile=FILE_PREFIX + 'server.pem', server_side=True)
    server.serve_forever()

except KeyboardInterrupt:
    print 'Shutting down the web server'
    server.socket.close()
