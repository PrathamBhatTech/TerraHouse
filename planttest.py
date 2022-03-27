import tensorflow as tf
import numpy as np

model = tf.keras.models.load_model("myplant.h5")
import cv2
from PIL import Image, ImageOps
import numpy as np



inp = Image.open("photo.jpg")
img = inp.resize((224,224))
img = np.array(img)/255.0
img = np.reshape(img, [1,224,224,3])
#features = conv_base.predict(img)

predictions = model.predict(img)
prediction = predictions.tolist()


if(prediction[0][0] > 0.5):
    print("Bacterial Spot")
elif(prediction[0][1] > 0.5):
    print("Early Blight")
elif(prediction[0][2] > 0.5):
    print("Healthy leaf")
elif(prediction[0][3] > 0.5):
    print("Late Blight")
elif(prediction[0][4] > 0.5):
    print("Leaf Mold")
elif(prediction[0][5] > 0.5):
    print("Mosaic Virus")
elif(prediction[0][6] > 0.5):
    print("Leaf Spot")
else:
    print("Healthy")


