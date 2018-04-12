import matplotlib.pyplot as plt
import numpy as np
import math
import imageio
from PIL import Image
from scipy import misc
import greyScale
import Convolution


def ideal_low_pass_filter(image, radius):
    idpf = np.zeros_like(image)
    h = len(image)
    w = len(image[0])
    for i in range(h):
        for j in range(w):
            if((i-h/2)**2 +(j-w/2)**2 < radius**2):
                idpf[i][j] = 1
    return idpf

def ideal_high_pass_filter(image, radius):
    idpf = np.zeros_like(image)
    h = len(image)
    w = len(image[0])
    for i in range(h):
        for j in range(w):
            if((i-h/2)**2 +(j-w/2)**2 > radius**2):
                idpf[i][j] = 1
    return idpf

def frequency_filter(image, kernel):
    a= np.array(image.shape)
    b = np.array(kernel.shape)
    kernel = np.fft.fft2(kernel, (a+b-1))
    F =np.fft.fft2(image, (a+b-1))
    G = np.multiply(F, kernel)
    g = np.fft.ifft2(G)
    g = g.real
    gsize = g.shape
    garray = np.array(gsize)
    xaxis = (garray-a)//2
    g = g[xaxis[0]: garray[0]-xaxis[0] -1,xaxis[1]:garray[1]-xaxis[1]-1]
    g = np.clip(g, 0, 1)
    return g

def gaussian_kernel(size = 3, std=1.0):
    s =(size-1)//2
    h = np.linspace(-s, s, size)
    h = np.exp(-h**2 / (2 * std**2))
    h = h * h[np.newaxis, :].T
    sumh = h.sum()
    if 0.0 != sumh:
        h /= sumh
    return h

def unsharp_masking(image):
    h = gaussian_kernel(20,5)
    I = make_impuls(20)
    kernel = I + 1* (I- h)
    return frequency_filter(image, kernel)

def make_impuls(size):
    I = np.zeros((size,size))
    I[size//2][size//2] = 1
    return I

def make_selective_filter(kernel):
    nykernel = np.zeros_like(kernel)
    print(nykernel.shape)
    for i in range(len(kernel)):
        for j in range(len(kernel[i])):
            if(kernel[i][j]> 0):
                nykernel[i][j] = 1
    return nykernel


#Trekke fra min verdi og dele på høyeste
if __name__ == "__main__":
    image = imageio.imread('./images/lochness.tiff').astype(np.float)/255
    noise = imageio.imread('./images/noise-a.tiff').astype(np.float)/255
    n = np.fft.fft2(noise)
    #imageio.imwrite('./images/noise-sprecter.png', np.log10(1+abs(np.fft.fftshift(n))))
    h = imageio.imread('./images/noise-sprecter.tiff').astype(np.float)/255
    h = np.fft.fftshift(greyScale.luminanceGrey(h))
    h = make_selective_filter(h)
    nyimage = frequency_filter(noise, h)
    plt.figure()
    plt.imshow(nyimage, cmap=plt.cm.gray)
    """avgGrey= greyScale.greyAvg(image)
    lumImg = greyScale.luminanceGrey(image)
    plt.figure()
    #plt.imshow(np.log10(1+abs(np.fft.fftshift(frequency_filter(lumImg, ideal_high_pass_filter(lumImg, 30))))), cmap=plt.cm.gray)
    g = unsharp_masking(lumImg)
    plt.imshow(g, cmap=plt.cm.gray)
    plt.figure()
    plt.imshow(lumImg, cmap=plt.cm.gray)"""
    plt.show()
