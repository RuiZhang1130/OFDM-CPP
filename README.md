# OFDM-CPP

## Test Video
[![Watch the video](https://raw.githubusercontent.com/RuiZhang1130/OFDM-CPP/master/build/source01.jpg)](https://youtu.be/PCwsLIZjMCs)  


## How to use?
In the `/build`, there is a picture "source01.jpg". You can use it for testing. Or choose any grayscale image you like, as long as you rename it to "source01.jpg".  
```
make
./myTest
```
A simple two-line code is used to run the program. Then you will get a newly generated audio file "test.wav". Now, you can play it using a speaker and record it with a microphone.
  
Record code:
```
arecord -f S32_LE -c1 -r44100 record.wav
```
Use `Ctrl+C` to end recording.  
Remember to save the recorded audio "record.wav" in the folder "build".
  
Run the program again.  
You will get a new image "output.jpg" and be exactly the same as the "source01.jpg".
  
