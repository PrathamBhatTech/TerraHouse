import imghdr
import streamlit as st
import pic
import tensorflow as tf
model = tf.keras.models.load_model("rotten.h5")

st.write(""" # Streamlit Deployment of Fruit Freshness Detection""")

# file = st.file_uploader("Please upload an image file", type=["jpg","png"])

import cv2
from PIL import Image, ImageOps
import numpy as np

def import_and_predict(image_data, model):
    # size = (100,100)
    # image_data = np.asarray(image_data)
    # # image = ImageOps.fit(image_data)#, size)# Image.ANTIALIAS)
    # # image = np.asarray(image)
    # img = cv2.cvtColor(image_data, cv2.COLOR_BGR2RGB)
    # img_resize = (cv2.resize(img, dsize=(100, 100),interpolation=cv2.INTER_CUBIC))/255.
    # img_reshape = img_resize[np.newaxis,...]
    # prediction = model.predict(img_reshape)
    inp = Image.open("photo.jpg")
    img =  inp.resize((100,100))
    img = np.array(img)/255.0
    img = np.reshape(img, [1,100,100,3])
    prediction = model.predict(img)
    return prediction


# if file is None:
#     st.text("Please upload an image file")

# else:
#     image = Image.open(file)
#     st.image(image, use_column_width = True)
#     prediction = import_and_predict(image, model)
#     if(prediction[0] > 0.5):
#         st.write("Rotten")
#     else:
#         st.write("Ripe")

#image = Image.open(file)
pic.take_input()
picture = cv2.imread('photo.jpg')
st.image(picture, use_column_width = True)
prediction = import_and_predict(picture, model)
if(prediction > 0.5):
    st.write("Rotten")
else:
    st.write("Ripe")