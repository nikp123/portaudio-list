#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>

int main(int argc, char *argv[]) {
	int err = Pa_Initialize();
	if(err != paNoError) {
		fprintf(stderr, "Error: unable to initilize portaudio %x\n", err);
		system("PAUSE");
	}

	int numOfDevices = Pa_GetDeviceCount();
	if(numOfDevices < 0) {
		fprintf(stderr, "Error: portaudio was unable to find a audio device! Code: 0x%x\n", numOfDevices);
		system("PAUSE");
	}
	for(int i = 0; i < numOfDevices; i++) {
		const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
		printf("Device #%d: %s\n\
\tInput Channels: %d\n\
\tOutput Channels: %d\n\
\tDefault SampleRate: %lf\n",\
		i+1, deviceInfo->name, deviceInfo->maxInputChannels,\
		deviceInfo->maxOutputChannels, deviceInfo->defaultSampleRate);
	}
	system("PAUSE");
	return 0;
}
