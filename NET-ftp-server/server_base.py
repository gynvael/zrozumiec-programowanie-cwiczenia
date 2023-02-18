import socket
import threading


class ServerThread(threading.Thread):
    def __init__(
        self, host, port, server_type=None, server_config=None, sock=None, **kwargs
    ):
        super().__init__()
        self.host = host
        self.port = port
        self.sock = sock
        self.server_type = server_type
        self.server_config = server_config
        self.queue = 3

    def run(self):
        _sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        _sock.bind((self.host, self.port))
        _sock.listen(self.queue)
        while True:
            self.sock, _ = _sock.accept()
            self.server_type(
                host=self.host, port=self.port, **self.server_config, sock=self.sock
            ).start()

    def stop(self):
        if self.sock:
            self.sock.close()

    def sendall(self, data):
        self.sock.sendall(data.encode())

    def recvall(self, amount):
        data_raw = self.sock.recv(amount)
        return data_raw.decode()

    def recvuntil(self, end, sock=None):
        buff = bytearray()
        s = self.sock if not sock else sock
        while end not in buff:
            new_byte = s.recv(1)
            if not new_byte:
                break
            buff.extend(new_byte)
        return buff.decode()
