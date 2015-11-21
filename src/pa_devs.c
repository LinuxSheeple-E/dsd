#include <stdio.h>

#ifdef USE_PORTAUDIO
#include <portaudio.h>

#ifdef WIN32
#include <windows.h>
#endif

/*******************************************************************/
static void PrintSupportedSampleRates(
 const PaStreamParameters *inputParameters,
 const PaStreamParameters *outputParameters )
 {
 static double sampleRates[] = {
 8000.0, 44100.0, 48000.0, -1 /* negative terminated list */
 };
 int i, printCount;
  PaError err;

 printCount = 0;
  for( i=0; sampleRates[i] > 0; i++ )
  {
  err = Pa_IsFormatSupported( inputParameters, outputParameters, sampleRates[i] );
  if( err == paFormatIsSupported )
  {
  if( printCount == 0 )
  {
  printf( "\t%8.2f", sampleRates[i] );
  printCount = 1;
  }
  else if( printCount == 4 )
  {
  printf( ",\n\t%8.2f", sampleRates[i] );
  printCount = 1;
  }
  else
  {
  printf( ", %8.2f", sampleRates[i] );
  ++printCount;
  }
  }
  }
  if( !printCount )
  printf( "None\n" );
  else
  printf( "\n" );
 }

void printPortAudioDevices()
{
    int     i, numDevices, defaultDisplayed;
    const   PaDeviceInfo *deviceInfo;
    PaStreamParameters inputParameters, outputParameters;
    PaError err;


    Pa_Initialize();

    printf( "\nPortAudio version number = %d\nPortAudio version text = '%s'\n",
            Pa_GetVersion(), Pa_GetVersionText() );


    numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );
        err = numDevices;
        goto error;
    }

    printf( "Number of devices = %d\n", numDevices );
    for( i=0; i<numDevices; i++ )
    {
        deviceInfo = Pa_GetDeviceInfo( i );
        printf( "--------------------------------------- device #%d\n", i );

    /* Mark global and API specific default devices */
        defaultDisplayed = 0;
        if( i == Pa_GetDefaultInputDevice() )
        {
            printf( "[ Default Input" );
            defaultDisplayed = 1;
        }
        else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultInputDevice )
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
            printf( "[ Default %s Input", hostInfo->name );
            defaultDisplayed = 1;
        }

        if( i == Pa_GetDefaultOutputDevice() )
        {
            printf( (defaultDisplayed ? "," : "[") );
            printf( " Default Output" );
            defaultDisplayed = 1;
        }
        else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultOutputDevice )
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
            printf( (defaultDisplayed ? "," : "[") );
            printf( " Default %s Output", hostInfo->name );
            defaultDisplayed = 1;
        }

        if( defaultDisplayed )
            printf( " ]\n" );

    /* print device info fields */
#ifdef WIN32
        {   /* Use wide char on windows, so we can show UTF-8 encoded device names */
            wchar_t wideName[MAX_PATH];
            MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName, MAX_PATH-1);
            wprintf( L"Name                        = %s\n", wideName );
        }
#else
        printf( "Name                        = %s\n", deviceInfo->name );
#endif
        printf( "Host API                    = %s\n",  Pa_GetHostApiInfo( deviceInfo->hostApi )->name );
        printf( "Max inputs = %d", deviceInfo->maxInputChannels  );
        printf( ", Max outputs = %d\n", deviceInfo->maxOutputChannels  );
        printf( "Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate );
  /* poll for supported sample rates */
  inputParameters.device = i;
  inputParameters.channelCount = deviceInfo->maxInputChannels;
  inputParameters.sampleFormat = paInt16;
  inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
  inputParameters.hostApiSpecificStreamInfo = NULL;
 
  outputParameters.device = i;
  outputParameters.channelCount = deviceInfo->maxOutputChannels;
  outputParameters.sampleFormat = paInt16;
  outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
  outputParameters.hostApiSpecificStreamInfo = NULL;
 
  if( inputParameters.channelCount > 0 )
  {
  printf("Supported sample rates\n for half-duplex 16 bit %d channel input = \n",
  inputParameters.channelCount );
  PrintSupportedSampleRates( &inputParameters, NULL );
  }
 
  if( outputParameters.channelCount > 0 )
  {
  printf("Supported sample rates\n for half-duplex 16 bit %d channel output = \n",
  outputParameters.channelCount );
  PrintSupportedSampleRates( NULL, &outputParameters );
  }
 
  if( inputParameters.channelCount > 0 && outputParameters.channelCount > 0 )
  {
  printf("Supported sample rates\n for full-duplex 16 bit %d channel input, %d channel output = \n",
  inputParameters.channelCount, outputParameters.channelCount );
  PrintSupportedSampleRates( &inputParameters, &outputParameters );
    }
  }

    Pa_Terminate();

    printf("----------------------------------------------\n");
    return;

error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
}

#else

void printPortAudioDevices()
{
    printf("PortAudio not supported in this build of dsd\n");
}

#endif
