import time
import json
import websocket
from PIL import Image
from io import BytesIO

class Camera():
   
    def __init__(self, topic, registry="127.0.0.1"):
        registry = websocket.create_connection(f"ws://{registry}:10101")
        cmd = "{'request': 'get', 'topic': '%s'}" % topic
        registry.send(cmd)
        data = registry.recv()
        response = json.loads(data)
        print(response)
        print(f"Connecting to ws://{response['address']}")
        self.connection = websocket.create_connection(f"ws://{response['address']}")
    
    def print_error(self, data):
       r = json.loads(data)
       print(f"Failed to grab the image: {r['error']['message']}")
       
    def grab(self):
        cmd = '{"method": "camera-grab-jpeg-binary"}'
        self.connection.send(cmd, websocket.ABNF.OPCODE_BINARY)
        data = self.connection.recv()
        if type(data) is str:
            self.print_error(data)
            return None
        else:
            return Image.open(BytesIO(data))
       
if __name__ == '__main__':
    camera = Camera("top-camera", "10.10.10.1")
    for i in range(10):
        image = camera.grab()
        if image != None:
            print(f"Saving top-camera-{i:05d}.jpg")
            image.save(f"top-camera-{i:05d}.jpg")
