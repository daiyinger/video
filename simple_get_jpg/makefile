objects = simple_jpg
src =  video_yuv420.c jpeg2rgb.c
simple_jpg: $(src) video_yuv420.o jpeg2rgb.o 
	g++ video_yuv420.o jpeg2rgb.o -o simple_jpg -L/usr/local/ffmpeg/lib -lx264 -ldl -pthread -lavformat -lavcodec -lavutil -lswscale
video_yuv420.o:video_yuv420.c
	gcc -c video_yuv420.c
jpeg2rgb.o:jpeg2rgb.c
	gcc -c jpeg2rgb.c 
info:
	g++ simple_code.cpp -o simple_code -lx264 -ldl -pthread
fscale:
	gcc -o fscale fscale.c -L/usr/local/ffmpeg/lib -lavformat -lavcodec -lavutil -lswscale
clean:
	rm -f video_yuv420.o 
	rm -f simple_code.o
	rm -f simple_video
	rm -f simple_code
	rm -f fscale
	rm -f fscale.o
	rm -f jpeg2rgb.o
