import os
import sys

mainv = sys.version[0]

if mainv < '3':
  from SocketServer     import TCPServer                as HTTPServer
  from SimpleHTTPServer import SimpleHTTPRequestHandler as HTTPRequestHandler
else:
  from http.server import HTTPServer
  from http.server import SimpleHTTPRequestHandler as HTTPRequestHandler

#def run(server_class=HTTPServer,
#        handler_class=BaseHTTPRequestHandler):
#    server_address = ('localhost', 8000)
#    httpd = server_class(server_address, handler_class)
#    httpd.serve_forever()

PORT = 80
listen = 'bferris-'
if os.name == 'posix':
  listen = 'bferris-n97'
elif os.name == 'nt':
  listen = 'bferris-pc'
else:
  print("Don't know what to listen on")
  sys.exit()

def run(server_class=HTTPServer, handler_class=HTTPRequestHandler):
    server_address = (listen, PORT)
    httpd = server_class(server_address, handler_class)
    httpd.serve_forever()

if __name__ == '__main__':

  print("Starting server at 8000")

  #Handler = HTTPServer.RequestHandler

  print("serving at port", PORT)
  run()
