# EdgeDetection-Algos
Implements two early computer vision algorithms, the Sobel and Canny edge detectors. 

## Canny Edge Detector

This edge detector uses a multi-stage algorithm to detect edges in images. 

The process consits of the following steps:
1. Applying the Gaussian filter to smooth the image in order to remove the noise in the image.
2. Find the intensity gradients of the image.
3. Apply non-maximum suppression to get rid of spurious response to edge detection.
4. Apply double threshold to determine potential edges.
5. Track edge by hysteresis: Finalize the detection of edges by suppressing all the other edges that are weak and not connected to strong edges.

### Raw Image vs Processed Image
![](https://upload.wikimedia.org/wikipedia/commons/2/20/%C3%84%C3%A4retuvastuse_n%C3%A4ide.png)

## Sobel Edge Detector

The sobel filter is an inproved edge detection algorithm.

The process consists of the folloring steps:
1. Convolution kernels for both axes.
2. Find absolute magnitude of directional gradients.
3. Using the direction of the gradient determine neighboring pixels to confirm edge.

### Raw Image vs Processed Image
![](https://cmmlin2016.files.wordpress.com/2013/11/sobel_result.jpg)


### Related Papers
[Sobel Edge Detection Algorithm](https://pdfs.semanticscholar.org/6bca/fdf33445585966ee6fb3371dd1ce15241a62.pdf)
