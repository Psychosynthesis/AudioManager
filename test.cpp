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

static const char VOLUME_RANGE_ERR[] = "Volume must be a fractional number ranging from 0 to 1";
static const char NO_ENDPOINTS_ERR[] = "No endpoints found.";
static const char OK_MSG[] = "Ok";

extern "C" int setVolume(double newVolume) {
  if (newVolume < 0 || newVolume > 1) {
    std::cout << VOLUME_RANGE_ERR << std::endl;
    return 1;
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
    std::cout << NO_ENDPOINTS_ERR << std::endl;
    return 2;
  }

  for (UINT i = 0; i < count; i++) {
    hr = pCollection->Item(i, &pEndpoint);
    hr = pEndpoint->GetId(&pwszID);
    IAudioEndpointVolume *endpointVolume = NULL;
    pEnumerator->GetDevice(pwszID, &pEndpoint);
    pEndpoint->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    /* Вариант получения свойств интерфейса и вывод его названия, для изменения громкости это не нужно
    hr = pEndpoint->OpenPropertyStore(STGM_READ, &pProps);
    PROPVARIANT varName;
    PropVariantInit(&varName);
    hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
    printf("Endpoint %d: \"%S\" (%S)\n", i, varName.pwszVal, pwszID);
    CoTaskMemFree(pwszID);
    pwszID = NULL;
    PropVariantClear(&varName);
    */
    endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL); // set volume level of device to 0.1 (10%)
    endpointVolume->Release();
    // you can save endpointVolume in a container to set set volume level for each device.
  }

  return 0;
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

extern "C" void voidExample() { printf("%s", "Hi! This is windows volume-changer dll example."); }
