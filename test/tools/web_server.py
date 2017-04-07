from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
import ssl

class RequestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        #self.protocol_version = 'HTTP/1.1'
        self.send_response(200)
        self.send_header('Content-type','text/html')
        self.end_headers()
        # Send the html message
        with open("content.txt", "r") as f:
            self.wfile.write(f.read())

try:
    server = HTTPServer(('localhost', 4443), RequestHandler)
    server.socket = ssl.wrap_socket(server.socket, certfile='server.pem', server_side=True)
    server.serve_forever()

except KeyboardInterrupt:
    print 'Shutting down the web server'
    server.socket.close()
