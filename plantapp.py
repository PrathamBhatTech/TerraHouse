import imghdr
import streamlit as st
#import pic
import tensorflow as tf
model = tf.keras.models.load_model("myplant.h5")

st.write(""" # Streamlit Deployment of Fruit Freshness Detection""")

#file = st.file_uploader("Please upload an image file", type=["jpg","png"])

import cv2
from PIL import Image, ImageOps
import numpy as np

def import_and_predict(image_data, model):
    
    inp = Image.open("photo.jpg")
    img = inp.resize((100,100))
    img = np.array(img)/255.0
    img = np.reshape(img, [1,100,100,3])
    prediction = model.predict(img)
    return prediction

#pic.take_input()
#picture = cv2.imread('photo.jpg')
#st.image(picture, use_column_width = True)

predictions = import_and_predict(picture, model)
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
