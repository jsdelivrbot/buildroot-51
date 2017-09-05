from http.server import HTTPServer, SimpleHTTPRequestHandler, BaseHTTPRequestHandler
from socketserver import TCPServer
import time
import psutil
import platform
import os.path
import sys
import re

server_start = time.time()
processor = platform.processor()
linux = platform.linux_distribution()[0] + " " + platform.linux_distribution()[1]


class MyRequestHandler(SimpleHTTPRequestHandler):
    def do_GET(self):
        
        current_time = time.localtime()
        elapsed_time = time.time() - server_start

        string_current_t = time.strftime("%a, %d %b %Y %H:%M:%S", current_time)
        #string_elapsed_t = time.strftime("%a, %d %b %Y %H:%M:%S", elapsed_time)

        cpu_usage = psutil.cpu_percent()
        memory_usage = psutil.virtual_memory()
        
        process_list = ''
    
        for process in psutil.process_iter():
            try:
                process_info = process.as_dict(attrs=['pid', 'name'])
                process_list += "PID " + str(process_info['pid']) + " " + "NAME " + process_info['name'] + "\n    "
            except psutil.NoSuchProcess:
                pass
        path = '/usr/bin/serverscripts/index_bkp.html'
        index_file = open(path)
        f = index_file.read()
        index_file.close()

        f = re.sub(r"hhh", process_list, f)
        f = re.sub(r"aaa", string_current_t, f)
        f = re.sub(r"bbb", str(elapsed_time), f)
        f = re.sub(r"ccc", processor, f)
        f = re.sub(r"ddd", str(cpu_usage), f)
        f = re.sub(r"eee", str(memory_usage), f)
        f = re.sub(r"ggg", linux, f)

        self.path = '/usr/bin/serverscripts/index.html'
        index_file = open(self.path, 'w')
        index_file.write(f)
        index_file.close()

        print("connection recieved")

        return SimpleHTTPRequestHandler.do_GET(self)


Handler = MyRequestHandler
server = TCPServer(('0.0.0.0', 8094), Handler)
server.serve_forever()
