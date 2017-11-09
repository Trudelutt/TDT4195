import numpy as np
import math

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
