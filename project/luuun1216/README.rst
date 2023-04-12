Face Recognition System
===========================
## Basic Information
Developing a Face Recognition System Using C++ and Python

**Github repository**: https://github.com/luuun1216/Face-Recognition-System

## Problem to Solve

Face recognition is a challenging task in computer vision that has numerous applications, such as security, surveillance, and human-computer interaction. Traditional methods for face recognition have limitations in terms of accuracy, efficiency, and scalability. Moreover, the emergence of deep learning has opened up new possibilities for improving the performance of face recognition systems, but it requires specialized knowledge and tools to build and train effective models. Therefore, the aim of this project is to develop a face recognition system that combines the strengths of C++ and Python and leverages deep learning to achieve high accuracy and efficiency, users only need to put the picture into the folder or open the camera, the system can training or show the final result.

## Prospective Users

The face recognition system has numerous potential users, including security and surveillance companies, law enforcement agencies, and government organizations. The system can be used for applications such as access control, identity verification, and criminal investigation. Moreover, the system can also be used in human-computer interaction scenarios, such as user identification. 

## System Architecture
The deep learning model will be built using a convolutional neural network (CNN) architecture, which has been shown to be effective for face recognition tasks. The model will be trained using a large dataset of face images, such as the popular Labeled Faces in the Wild (LFW) dataset, and will use techniques such as data augmentation, regularization, and optimization to improve its performance.

Once the model is builded, the face recognition system will use it to perform the recognition task on new images. The system will first pre-process the input image using C++ functions to normalize the image, crop the face region, and extract features. The pre-processed image will then be passed to the Python code for classification using the trained deep learning model. Finally, the system will display the results, such as the detected face and the corresponding identity.

![image](https://github.com/luuun1216/Face-Recognition-System/blob/main/Architecture.png)

## API Description

The Face Recognition API consists of several modules, each with its own set of functions and parameters:

1 . Image Processing: This module provides functions for loading, resizing, cropping, and normalizing face images. Developers can use these functions to preprocess images before passing them to the feature extraction module.

2 .Feature Extraction: This module provides functions for extracting facial features from input images. The functions use deep learning models to extract features such as face landmarks, embeddings, and descriptors.

3 .Model Inference: This module provides functions for loading, training, and evaluating deep learning models. Developers can use these functions to train their own models or use pre-trained models for face recognition.

4 . Result Display: This module provides functions for visualizing the results of the face recognition system. Developers can use these functions to display the detected faces, recognized faces, and confidence scores in the input images.

To use the Face Recognition API, developers need to first install the necessary software libraries and dependencies, including C++, Python, Pybind11, and the deep learning framework (such as PyTorch or TensorFlow). They can then import the API modules into their own code and call the functions as needed.

## Engineering Infrastructure
### Automatic build system and how to build your program
- Use Conda or ip to install the dependent tools or library

### Version Control

- Git

### Programing Language

- Face image crop, Camera function and image pre-processing function will be completed by C++.

- Face reconition function and Generate the report function will be completed by Python.

### Make-Pybind-Pytest

- The build system will convert the c++ function into python function through make and pybind.

- The python code will be tested by pytest.


## Schedule

| Week | Schedule |                                                                                                     
| ------------- | ------------- |
| Week 1: Project Planning and Setup  | 1. Define project scope and requirements<br />2. Plan project schedule and milestones<br />3. Set up development environment (install C++, Python, Pybind11, and necessary libraries)<br /> |
| Week 2: Image Pre-Processing | 1. Implement image loading and pre-processing functions in C++<br />2. Test image pre-processing functions and optimize for performance<br />|
| Week 3: Feature Extraction | 1. Implement feature extraction functions in C++ <br />2. Test feature extraction functions and optimize for performance<br />|
| Week 4: Deep Learning Model Training | 1. Develop deep learning model using Python and a deep learning framework (such as PyTorch or TensorFlow) |
| Week 5: Model Integration | 1. Use Pybind11 to integrate the C++ image pre-processing and feature extraction functions with the Python model training and inference code<br />2. Test integrated model |
| Week 6: Face Recognition System Implementation |1. Implement the complete face recognition system, including the C++ and Python code for image pre-processing, feature extraction, model inference, and result display |
| Week 7: System Evaluation and build workflow | 1. Evaluate the system performance and accuracy using benchmark datasets and metrics<br />2. Write Makefile or requirement.txt |
| Week 8: Final check and prepare presentation  | 1. Final check<br /> 2. Think about what I need to say |

## References
1. [Face Recognition] https://github.com/ageitgey/face_recognition
2. [Real-time Face recognition] https://github.com/s90210jacklen/Real-time-Face-recognition
3. [FaceNet: A Unified Embedding for Face Recognition and Clustering] https://arxiv.org/pdf/1503.03832.pdf
4. [Face Detection using Haar Cascades] https://docs.opencv.org/3.4/d2/d99/tutorial_js_face_detection.html
