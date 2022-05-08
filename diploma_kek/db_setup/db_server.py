import zmq


class ZMQServer:
    CONTEXT = zmq.Context()

    _SOCKET = "tcp://*:5555"

    def __init__(self) -> None:
        self.socket = self.CONTEXT.socket(zmq.REP)
        self.socket.bind(self._SOCKET)

    def run(self) -> None:
        while True:
            message = self.socket.recv()

            # TODO

            # socker.send(b"")


server = ZMQServer()
server.run()
