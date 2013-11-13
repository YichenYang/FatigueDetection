FatigueDetection
================

Fatigue driving is one of the main factors in causing traffic accidents.Since fatigue detection based on human eye state is the most direct way. An fatigue detection system is built based on DSP (TMS320DM6437). An infrared light supplement camera is adopted as the system's input device. The fatigue detection algorithm is based on the analysis of video.

An AdaBoost cascade classification of Harr features is applied to locate the driver's face and eyes. The particle filter algorithm is applied to track eyes. Image binarization, projection method and adaptive updating algorithm are applied to extract the state of eyes. The scheme can not only locate the center of the driver's eyeballs' in good condition of light, but also give the proportion of eyes open. As for the image after the light is supplemented, the centers of eyeballs' can be located accurately and the state of eyes is also estimated.

The scheme's computational complexity is reduced owing to the algorithm only search through all the pixels. The experiment results show that the system can accurately monitor the drivers' fatigue degree in real-time.



