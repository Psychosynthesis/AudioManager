#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include <Windows.h>
#include <mmsystem.h>

#include <initguid.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h> // Need to use PKEY_Device_FriendlyName

static inline char *getCstring(std::string stringToConvert) {
  char *result = new char[stringToConvert.length() + 1];
  strcpy(result, stringToConvert.c_str());
  return result;
}

static const char VOLUME_RANGE_ERR[] = "Volume must be a fractional number ranging from 0 to 1";
static const char NO_ENDPOINTS_ERR[] = "No endpoints found.";
static const char OK_MSG[] = "OK";

extern "C" char *setVolume(double newVolume) {
  if (newVolume < 0 || newVolume > 1) {
    std::cout << VOLUME_RANGE_ERR << std::endl;
    char* ret = getCstring(VOLUME_RANGE_ERR);
    return ret;
  }

  HRESULT hr = S_OK;
  UINT count = 0;
  IMMDeviceEnumerator *pEnumerator = NULL;
  IMMDeviceCollection *pCollection = NULL;
  IMMDevice *pEndpoint = NULL;
  IPropertyStore *pProps = NULL;
  LPWSTR pwszID = NULL;

  CoInitialize(NULL);
  hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL,CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
  hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE,&pCollection); // ** use "eCapture" for microphones and "eRender" for speakers.
  hr = pCollection->GetCount(&count);

  if (count == 0) {
    char* ret = getCstring(NO_ENDPOINTS_ERR);
    return ret;
  }

  for (UINT i = 0; i < count; i++) {
    hr = pCollection->Item(i, &pEndpoint);
    hr = pEndpoint->GetId(&pwszID);
    IAudioEndpointVolume *endpointVolume = NULL;
    pEnumerator->GetDevice(pwszID, &pEndpoint);
    pEndpoint->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    hr = pEndpoint->OpenPropertyStore(STGM_READ, &pProps);
    PROPVARIANT varName;
    PropVariantInit(&varName);
    hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
    printf("Endpoint %d: \"%S\" (%S)\n", i, varName.pwszVal, pwszID);
    CoTaskMemFree(pwszID);
    pwszID = NULL;
    PropVariantClear(&varName);
    endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL); // set volume level of device to 0.1 (10%)
    endpointVolume->Release();
    // you can save endpointVolume in a container to set set volume level for each device.
  }

  std::cout << OK_MSG << std::endl;
  char* ret = getCstring(OK_MSG);
  return ret;
}

extern "C" void showDevices() {
  UINT numDevices = waveInGetNumDevs();
  std::cout << "Number of input devices: " << numDevices << std::endl;

  for (UINT i = 0; i < numDevices; i++) {
      WAVEINCAPS deviceInfo;
      if (waveInGetDevCaps(i, &deviceInfo, sizeof(deviceInfo)) == MMSYSERR_NOERROR) {
          std::cout << "Input device " << i << ": " << deviceInfo.szPname << std::endl;
          std::cout << "Channels: " << deviceInfo.wChannels << std::endl;
          std::cout << std::endl;
      }
  }

  numDevices = waveOutGetNumDevs();
  std::cout << "Number of output devices: " << numDevices << std::endl;

  for (UINT i = 0; i < numDevices; i++) {
      WAVEOUTCAPS deviceInfo;
      if (waveOutGetDevCaps(i, &deviceInfo, sizeof(deviceInfo)) == MMSYSERR_NOERROR) {
          std::cout << "Output device " << i << ": " << deviceInfo.szPname << std::endl;
          std::cout << "Channels: " << deviceInfo.wChannels << std::endl;
          std::cout << std::endl;
      }
  }
}

extern "C" const char *concatenateStrings(const char *str1, const char *str2) {
  std::string result = std::string(str1) + std::string(str2);
  char *cstr = new char[result.length() + 1];
  strcpy(cstr, result.c_str());
  return cstr;
}

extern "C" char **createArrayString(char **arr, int size) {
  char **vec = (char **)malloc((size) * sizeof(char *));
  for (int i = 0; i < size; i++) {
    vec[i] = arr[i];
  }
  return vec;
}

extern "C" void voidExample() { printf("%s", "hello world"); }
