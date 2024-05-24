like it says


essential reading


https://www.hackster.io/mjrobot/computer-vision-at-the-edge-with-grove-vision-ai-module-v2-0003c7



# good site The main sensecraft site. You don't need to put your address


https://sensecraft.seeed.cc/ai/#/device/local



convert your edgeimpulse int8 quantized model from  .lite   to .tflite   (just change the name then do the following on a google colab)

```
!pip install ethos-u-vela
!!vela --version
!ls
!vela ei-periquito-vs-robot-int8.tflite --accelerator-config ethos-u55-64

```

The last command should allow uploading the .tflite labelled file and it downloading the proper .tflite file that sensecraft can then install on your grove vision V2 ai
