// C libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// extern libs
#include <math.h>
#include <portaudio.h>

void waitKey(void){
	#ifdef __unix__
	system("read -r -p \"Press any key to continue...\" key");
	#endif
	#ifdef __WIN32__
	system("PAUSE");
	#endif
}

void quit(int status) {
	waitKey();
	exit(status);
}

void info(void) {
#ifdef __WIN32__
	printf("Hello, this is the CAVA portaudio config utility!\n\n\
This is used to easily configure CAVA\'s input device. \
After continuing you will be prompted to select a desired device.\n\n
For Windows users: select the one of which name begins with Stereo mix, \
if you don\'t have it, don\'t proceed and get better sound drivers\
and run this utility again (it should be in the same directory as CAVA)\n"
	waitKey();
#endif
}

int main(int argc, char *argv[]) {
	info();

	int selection = 0, err = Pa_Initialize();
	if(err != paNoError) {
		fprintf(stderr, "Error: unable to initilize portaudio %x\n", err);
		quit(-1);
	}

	int numOfDevices = Pa_GetDeviceCount();
	if(numOfDevices < 0) {
		fprintf(stderr, "Error: portaudio was unable to find a audio device! Code: 0x%x\n", numOfDevices);
		quit(-1);
	}
	for(int i = 0; i < numOfDevices; i++) {
		const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);

		// skip devices don't have input
		if(deviceInfo->maxInputChannels==0) continue;
		
		printf("Device #%d: %s\n\
\tInput Channels: %d\n\
\tDefault SampleRate: %lf\n",\
		i+1, deviceInfo->name, deviceInfo->maxInputChannels,\
		deviceInfo->defaultSampleRate);
	}

	while(1) {
		printf("Please select the input device or 0 for automatic selection: ");
		if(!scanf("%u", &selection)) puts("Must be a number!");
	 	else if(selection>numOfDevices) puts("Invalid device range");
		else if(selection!=0&&!Pa_GetDeviceInfo(selection-1)->maxInputChannels) puts("Selected device doesn't have any inputs");
		else break;
	}


	char configPath[255], number[100], *tempStr, *backup;
#ifdef __WIN32__
	strcpy(configPath, getenv("APPDATA"));
	strcat(configPath, "\\cava\\config");
#endif
#ifdef __unix__
	tempStr = getenv("XDG_CONFIG_HOME");
	if(!tempStr) {
		tempStr = getenv("HOME");
		if(!tempStr) {
			puts("User is homeless, cannot continue!");
			quit(-1);
		}
		sprintf(configPath, "%s/.config/cava/config", tempStr);
	} else sprintf(configPath, "%s/cava/config", tempStr);
#endif
	printf("Saving changes to: %s\n", configPath);

	FILE *configFile = fopen(configPath, "rb+");
	while(1) {
		if(!fgets(number, 100, configFile)) {
			fprintf(stderr, "Line not found!");
			quit(-1);
		}
		if(!strncmp("source", number, 6)) break;
	}

	// position and lenght of the old file
	int position = ftell(configFile), lenght;
	
	// backup everything after
	fseek(configFile, 0, SEEK_END);
	lenght = ftell(configFile);
	tempStr = (char*)malloc(sizeof(char)*(lenght-position));
	fseek(configFile, position, SEEK_SET);
	fread(tempStr, sizeof(char), lenght-position, configFile);

	// write changed line
	fseek(configFile, position-strlen(number), SEEK_SET);
	if(selection) fprintf(configFile, "source = %d\n", selection-1);
	else fprintf(configFile, "source = auto\n");
	
	// backup everything before because C can't apparently append from position, it can only create new or append to them
	int pos2 = ftell(configFile);
	backup = malloc(sizeof(char)*pos2);
	fseek(configFile, 0, SEEK_SET);
	fread(backup, sizeof(char), pos2, configFile);
	fclose(configFile);
	
	// assemble back the file	
	configFile = fopen(configPath, "wb");
	fwrite(backup, sizeof(char), pos2, configFile);
	fwrite(tempStr, sizeof(char), lenght-position, configFile);
	
	// clean up this mess
	fclose(configFile);
	free(backup);
	free(tempStr);
	return 0;
}
