# NodeJS AudioManager
Control your computer's sound from any device on the network, without any additional applications. \
You nedd G++ ([get it here](http://www.equation.com/servlet/equation.cmd?fa=fortran), i prefer v.11) to be installed for make changes in DLL.\
Feel free to change an interface [in index.html](public/index.html) as you wish.

## Install and run
Just copy the repository and install the dependencies: \
`npm install --production`

## How it work?
This application runs C code with WinAPI-calls in Node using a third-party library. Unlike most other applications that use the node-ffi or [node-ffi-napi](https://github.com/node-ffi-napi/node-ffi-napi) libraries for this purpose, which requires node-gyp to compile (and that works, to put it mildly, disgustingly), this option uses [node-ffi-rs](https://github.com/zhangyuang/node-ffi-rs) and G++ compiler, what eliminates the need to install a bunch of garbage into the system (for example, you don’t need Python and ms-build-tools for this).

There is some simillar libs for same goals, like [win-audio](https://github.com/fcannizzaro/win-audio), or [node-audio-windows](https://github.com/jlonardi/node-audio-windows), but both of them also need a trash to build (windows-build-tools, python and so on).

In order not to run this yourself every time, the [node-windows](https://www.npmjs.com/package/node-windows) package is used here, which makes an `exe`-wrapper for the script and allows you to install it on the system as a service. After installing this package, use the command to install the service: `npm run setup` \
For removing: `npm run uninstall`

## Build dll:
`g++ -shared -o test.dll test.cpp -lole32 -loleaut32 -lwinmm` \
`lwinmm` here need for `waveInGetNumDevs`-function

Starting the server after installation: \
`npm run start`
