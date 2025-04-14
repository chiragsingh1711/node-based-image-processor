# node-based-image-processor

## Installation Steps

1.  Download and Extract OpenCV

    - Download OpenCV 4.11.0 from the official OpenCV website

    - Extract the downloaded archive to C:/opencv

2.  Configure Environment Variables
    Add the OpenCV bin directory to your system's PATH environment variable:

    - Right-click on "My Computer" or "This PC" and select "Properties"

    - Click on "Advanced system settings"

    - Click on "Environment Variables"

    - Under "System variables", find and select "Path", then click "Edit"

    - Add the path to the OpenCV bin directory (e.g., C:\opencv\build\x64\vc16\bin)

    - Click "OK" to save changes

3.  Create a New Visual Studio Project
    Open Visual Studio

    - Create a new empty C++ project

    - Ensure the platform is set to x64 (not Win32)

4.  Configure Project Properties
    For All Configurations (Debug and Release):

    1. C++/General → Additional Include Directories:

       - Add C:\opencv\build\include

    2. Linker/General → Additional Library Directories:

       - Add C:\opencv\build\x64\vc16\lib (adjust according to your Visual Studio version)

    3. Linker/Input → Additional Dependencies:

       - For Debug configuration: opencv_world4110d.lib

       - For Release configuration: opencv_world4110.lib

    4. Debugging → Environment:

       - Add PATH=C:\opencv\build\x64\vc16\bin;%PATH%

## Building and Running

- Select the appropriate configuration (Debug or Release) and platform (x64)

- Build the solution (F7 or Build → Build Solution)

- Run the program (F5 or Debug → Start Debugging)

## Brightness/Contrast Node

1. Adjust image brightness with a slider (-100 to +100)
2. Adjust image contrast with a slider (0 to 3)
3. Provide reset buttons for each parameter

```c++
BrightnessContrastNode("Brightness/Contrast", 1.2f, 10.0f);
```

![Alt text](images/BrightnessContrast.png)

```c++
// Brightness and contrast OpenCV Function
// Formula: output = alpha * input + beta
inputImage.convertTo(outputImage, -1, m_alpha, m_beta);
```

## Color Channel Splitter

1. Split RGB/RGBA image into separate channel outputs
2. Option to output grayscale representation of each channel

(Add a before after image here)

## Blur Node

1. Implement Gaussian blur with configurable radius (1-20px)
2. Option for uniform or directional blur
3. Include preview of kernel for educational purposes

(Add a before after image here)

## Threshold Node

1. Convert to binary image based on threshold value
2. Include options for different thresholding methods (binary, adaptive, Otsu)
3. Display histogram of image to assist with threshold selection

(Add a before after image here)

## Edge Detection Node

1. Implement both Sobel and Canny edge detection algorithms
2. Allow configuration of parameters (thresholds, kernel size)
3. Option to overlay edges on original image

(Add a before after image here)

## Blend Node

1. Combine two images using different blend modes
2. Support at least 5 blend modes (normal, multiply, screen, overlay, difference)
3. Include opacity/mix slider

(Add a before after image here)

## Noise Generation Node

1. Create procedural noise patterns (Perlin, Simplex, Worley)
2. Allow configuration of noise parameters (scale, octaves, persistence)
3. Option to use noise as displacement map or direct color output

(Add a before after image here)

## Convolution Filter Node

1. Provide a 3x3 or 5x5 matrix for custom kernel definition
2. Include presets for common filters (sharpen, emboss, edge enhance)
3. Display visual feedback of the kernel effect

(Add a before after image here)
