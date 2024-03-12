import socket
import ctypes as ct
import struct
import random
import time
from threading import Thread

# localhost
ip = "192.168.0.239"
port = 44000

socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
socket.bind((ip, port))

class Request(ct.Structure):
    _pack_ = 1
    _fields_ = (('marker', ct.c_uint32),
                ('channel', ct.c_uint16),
                ('response_port', ct.c_uint16),
                ('checksum', ct.c_uint16))
    size = 10

    def __str__(self):
        return f'marker {self.marker}, channel {self.channel}, response port {self.response_port}, checksum {self.checksum}'

    def from_bytes(bytes):
        return Request.from_buffer_copy(bytes)

class Response(ct.Structure):
    _pack_ = 1
    _fields_ = (('marker', ct.c_uint16),
                ('channel', ct.c_uint8),
                ('state', ct.c_uint8),
                ('voltage', ct.c_uint32),
                ('current', ct.c_uint32))
    size = 12

    def from_buffer(bytes):
        return Response.from_buffer_copy(bytes)

class ChannelData(object):
    def __init__(self, voltage_range, current_range):
        self.state = False
        self.voltage = 0
        self.current = 0
        self.voltage_range = voltage_range
        self.current_range = current_range

    def __str__(self):
        return f"state: {self.state}, voltage: {self.voltage}, current: {self.current}"

    def enable(self):
        self.state = True
        # random voltage in range
        self.voltage = random.uniform(self.voltage_range[0], self.voltage_range[1])
        # random current in range
        self.current = random.uniform(self.current_range[0], self.current_range[1])

    def disable(self):
        self.state = False
        self.voltage = 0
        self.current = 0

    def shuffle(self):
        # increase or decrease voltage by 1% of range
        self.voltage += (self.voltage_range[1] - self.voltage_range[0]) * 0.01
        # increase or decrease current by 1% of range
        self.current += (self.current_range[1] - self.current_range[0]) * 0.01

channels = {
    "1": ChannelData((14.0, 15.0), (0, 100)),
    "2": ChannelData((14.0, 15.0), (0, 100)),
    "3": ChannelData((14.0, 15.0), (0, 100)),
    "4": ChannelData((14.0, 15.0), (0, 100)),
    "5": ChannelData((14.0, 15.0), (0, 100)),
    "6": ChannelData((4.0, 5.0), (0, 400)),
    "7": ChannelData((4.0, 5.0), (0, 400)),
}

# voltage = sum of channels voltages
# zero_channel = ChannelData(False, )

channels["1"].enable()
channels["6"].enable()

def shuffle():
    reset_state = 0
    while True:
        for channel in channels:
            if channels[channel].state:
                channels[channel].shuffle()
        if channels["1"].state == False:
            reset_state += 1
        if reset_state == 10:
            channels["1"].enable()
            reset_state = 0
        time.sleep(0.5)

stream = False
tgt = ('127.0.0.1', 25565)

def receive():
    while True:
        print("waiting for request")
        message, address = socket.recvfrom(1024)
        request = Request.from_bytes(message)
        print(f"received request: {request}")
        if request.marker == 0xAAAAAAAA:
            if channels.__contains__(str(request.channel)):
                if channels[str(request.channel)].state:
                    channels[str(request.channel)].disable()
                else:
                    channels[str(request.channel)].enable()
        else:
            print('invalid marker')
        global tgt
        global stream
        tgt = (address[0], request.response_port)
        stream = True

def send():
    while True:
        if not stream:
            time.sleep(0.5)
            continue
        b = bytearray()
        for i in range(12):
            response = Response()
            response.channel = i
            if channels.__contains__(str(i)):
                response.state = int(channels[str(i)].state)
                response.voltage = int(channels[str(i)].voltage * 1000)
                response.current = int(channels[str(i)].current)
            else:
                response.state = 0
                response.voltage = 0
                response.current = 0
            response.marker = 0xBBBBBBBB
            response.checksum = 0
            b = b + bytearray(response)

        print(f'sent data to {tgt}')
        socket.sendto(b, tgt)
        time.sleep(0.5)


if __name__ == '__main__':
    thread = Thread(target = shuffle)
    thread2 = Thread(target = receive)
    thread3 = Thread(target = send)
    thread.start()
    thread2.start()
    thread3.start()
    thread.join()
    thread2.join()
    thread3.join()
