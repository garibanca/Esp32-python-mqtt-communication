import paho.mqtt.client as mqtt
import cv2
import numpy as np
import _thread



import io
import os
import time

def gidenden(mus):
    pata = input("başlık seç 1 :camera, 2: my/test/topic, diğer :boşver")
    if len(pata) == 0: return
    elif(pata =="1"):
        kata = input("kamera verisi gir")
        if len(kata) == 0: return
        mus.publish("camera", kata)
    elif(pata==2):
        kata = input("kamera verisi gir")
        if len(kata) == 0: return
        mus.publish("kamera", kata)
    else:
        print("geçtin")



def convertToPPM(im):
	with io.BytesIO() as f:
		im.save(f, format='ppm')
		f.seek(0)
		return f.getvalue()

def uniquify(path):
    filename, extension = os.path.splitext(path)
    counter = 1

    while os.path.exists(path):
        path = filename + "(" + str(counter) + ")" + extension
        counter += 1

    return path

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected successfully")
    else:
        print("Connect returned result code: " + str(rc))

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	if (msg.topic == "kamera"):

		print("sorun yok")
		print(str(len(msg.payload))+"                  "+ str(len(msg.payload)))
		image_data = msg.payload # byte values of the imag
		
		nparr = np.frombuffer(image_data, np.uint8)
		imagem = cv2.imdecode(nparr, -1)
		
		
		face_cascade = cv2.CascadeClassifier('/usr/local/lib/python3.8/dist-packages/cv2/data/haarcascade_frontalface_default.xml')
		gray = cv2.cvtColor(imagem, cv2.COLOR_BGR2GRAY)
		faces = face_cascade.detectMultiScale(gray, 1.1, 4)
		for (x, y, w, h) in faces:
			cv2.rectangle(imagem, (x, y), (x+w, y+h), (255, 0, 0), 2)
		
		cv2.imshow('image',imagem)
		cv2.waitKey(100)
		
		time.sleep(0.1)
		client.publish("istek", "Hello")
		
		
	else:
		print("Received message: " + msg.topic + " -> " + msg.payload.decode("utf-8"))



def on_disconnect(client, userdata,rc=0):
    logging.debug("DisConnected result code "+str(rc))
    client.loop_stop()

# create the client
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect

# enable TLS
client.tls_set(tls_version=mqtt.ssl.PROTOCOL_TLS)

# set username and password
client.username_pw_set("esplerle", "Zz19071990")

# connect to HiveMQ Cloud on port 8883
client.connect("694e712811a34dd8802e5445ec37cd96.s2.eu.hivemq.cloud", 8883)

# subscribe to the topic "my/test/topic"
client.subscribe("my/test/topic")
client.subscribe("kamera")
client.subscribe("istek")

# publish "Hello" to the topic "my/test/topic"
client.publish("my/test/topic", "Hello")
#client.loop_stop(0.1)
# Blocking call that processes network traffic, dispatches callbacks and handles reconnecting.
client.publish("istek", "Hello")
#client.loop_forever()
#_thread.start_new_thread( client.loop_forever, )
client.loop_start()
while True:
    gidenden(client)
