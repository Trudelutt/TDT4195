import numpy as np

def greyAvg(image):
    gray = np.zeros((image.shape[0], image.shape[1]), dtype= np.float)
    gray = (image[...,0]+image[...,1]+image[...,2])/3
    return gray

def luminanceGrey(image):
    gray = np.zeros((image.shape[0], image.shape[1]), dtype= np.float)
    gray = (0.2126*image[...,0]+0.7152*image[...,1]+0.0722*image[...,2])
    return gray
