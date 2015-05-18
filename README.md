
Emerald
==========
A small graphics engine using DirectX 11

Requirement
----------
```
Microsoft DirectX SDK (June 2010) 
```
```
freetype-2.5.3 
```
```
ffmpeg-20140723 
```
Usage
----------
### include
```
#include "Emerald.h"
```
### init
```
EEDesc desc;
desc.applicationName = L"Emerald";
desc.isFullScreen = false;
desc.width = 800;
desc.height = 450;
desc.isSSAA = true;
desc.isVsync = false;
EEInitialize(desc);
```
You could also do that in this way:
```
EEInitialize();
```
### run
```
while (EERun())
{
    EEBeginScene(EEColor::WHITE);
    // todo
    EEEndScene();
}
```
### shutdown
```
EEShutdown();
```
