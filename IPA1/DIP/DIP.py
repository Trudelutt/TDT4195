import matplotlib.pyplot as plt
import numpy as np
import math
from PIL import Image
from scipy import misc

def greyAvg(image):
    gray = np.zeros((image.shape[0], image.shape[1]), dtype= np.float)
    gray = (image[...,0]+image[...,1]+image[...,2])/3
    return gray

def luminanceGrey(image):
    gray = np.zeros((image.shape[0], image.shape[1]), dtype= np.float)
    gray = (0.2126*image[...,0]+0.7152*image[...,1]+0.0722*image[...,2])
    return gray

def trans(image):
    trans = np.zeros((image.shape[0], image.shape[1]), dtype= np.float)
    for i in range(len(image)):
        for j in range(len(image[i])):
            trans[i][j] = 255 -image[i][j]
    return trans

def gammaTrans(image, gamma):
    gammaTrans = np.zeros((image.shape[0], image.shape[1]), dtype= np.float)
    gammaTrans = image**gamma
    return gammaTrans

def convolution(image, filter):
    conv = np.zeros((image.shape[0], image.shape[1]), dtype= np.float)
    #Add pading
    padImage = np.pad(image, (len(filter)//2, len(filter)//2),'constant')
    #Flipp
    flipped = np.zeros((filter.shape[0], filter.shape[1]), dtype= np.float)
    for i in range(len(filter)):
        for j in range(len(filter[i])):
            flipped[i][j] = filter[len(filter)-i-1][len(filter[i])-j-1]
    #Make the filter move correct throw the img
    for i in range(len(padImage)- len(flipped)+1):
        for j in range(len(padImage[0])-len(flipped)+1):
            sum = 0
            for row in range(len(flipped)):
                for col in range(len(flipped[row])):
                    #sum det value un the filter multiplied with the corresponding value
                    sum+= flipped[row][col]*padImage[row+i][col+j]
            conv[i][j] = sum
    return conv

def convolutionColor(image, filter):
    convolutionColor = np.zeros_like(image)
    convolutionColor[...,0] = np.abs(convolution(image[...,0], filter))
    convolutionColor[...,1] = np.abs(convolution(image[...,1], filter))
    convolutionColor[...,2] = np.abs(convolution(image[...,2], filter))
    return convolutionColor


def magnitudeOfGradient(image, sobelX, sobelY):
    magnitude = np.zeros_like(image)
    horizontalImage = convolution(image, sobelX)
    verticalImage = convolution(image, sobelY)
    for i in range(len(image)):
        for j in range(len(image[i])):
            magnitude[i][j] = math.sqrt((horizontalImage[i][j]**2) + verticalImage[i][j]**2)
    return magnitude

avgKernel = np.ones((3,3))/9
gaussianKernel = np.array([[1, 4, 6, 4, 1],
                          [4, 16, 24, 16, 4],
                          [6, 24, 36, 24, 6],
                          [4, 16, 24, 16, 4],
                          [1, 4, 6, 4, 1]])/256

sobelX = np.array([[-1,0, 1],
             [-2, 0, 2],
             [-1, 0, 1]])
sobelY = np.array([[-1, -2, -1],
                  [0, 0, 0],
                  [1, 2, 1]])

image = misc.imread('./images/4.1.07-jelly-beans.tiff').astype(np.float)/255
avgGrey= greyAvg(image)
lumImg = luminanceGrey(image)
#t = trans(lumImg)
#g = gammaTrans(lumImg,10)
#c = convolution(lumImg, avgKernel)
#cc = convolutionColor(image, avgKernel)
m = magnitudeOfGradient(lumImg, sobelX, sobelY)
plt.figure()
plt.imshow(m, cmap=plt.cm.gray)
plt.show()
