Grove with XIAO-ESP32S3 

Grove can be programmed using the IDF Matter programmer
https://wiki.seeedstudio.com/getting_started_with_matter/


or with AT commands
https://wiki.seeedstudio.com/grove_vision_ai_v2_at/


XIAO ESP32S3 needs this board loaded onto the Arduino IDE. If you have multiple boards put a comma no space between them.

```https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json```



Demos for both

https://wiki.seeedstudio.com/grove_vision_ai_v2_demo/



essential reading


https://www.hackster.io/mjrobot/computer-vision-at-the-edge-with-grove-vision-ai-module-v2-0003c7



# good site The main sensecraft site. You don't need to put your address


https://sensecraft.seeed.cc/ai/#/device/local



convert your edgeimpulse int8 quantized model from  .lite   to .tflite   (just change drag and drop onto google colab then change the extension from .loite to .tflite)

```
!pip install ethos-u-vela
!!vela --version
```
Now upload your int8 quat file with new extension .tflite and check that it is there in the main upload area
```
!ls
```
Then run this command, replacing filename with your file name

```
!vela filename.tflite --accelerator-config ethos-u55-64

```

The last command should allow uploading the .tflite labelled file and it downloading the proper .tflite file that sensecraft can then install on your grove vision V2 ai




My Colab is at 

https://colab.research.google.com/drive/1y6_UrLV31ou59T00OmHwqDJIT-tcbCMo?usp=sharing


