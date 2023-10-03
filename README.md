# NodeJS AudioManager
Control your computer's sound from any device on the network, without any additional applications. \
You need G++ ([get it here](http://www.equation.com/servlet/equation.cmd?fa=fortran), i prefer v.11) to be installed for make changes in DLL.\
Feel free to change an interface [in index.html](public/index.html) as you wish.

## Install and run
Just copy the repository and install the dependencies: \
`npm install --production`

## How it work?
This application runs C code with WinAPI-calls in Node using a third-party library. Unlike most other applications that use the node-ffi or [node-ffi-napi](https://github.com/node-ffi-napi/node-ffi-napi) libraries for this purpose, which requires node-gyp to compile (and that works, to put it mildly, disgustingly), this option uses [node-ffi-rs](https://github.com/zhangyuang/node-ffi-rs) and G++ compiler, what eliminates the need to install a bunch of garbage into the system (for example, you don’t need Python and ms-build-tools for this).

There is some simillar libs for same goals, like [win-audio](https://github.com/fcannizzaro/win-audio), or [node-audio-windows](https://github.com/jlonardi/node-audio-windows), but both of them also need a trash to build (windows-build-tools, python and so on).

In order not to run this yourself every time, the [node-windows](https://www.npmjs.com/package/node-windows) package is used here, which makes an `exe`-wrapper for the script and allows you to install it on the system as a service. After installing this package, use the command to install the service: `npm run setup` \
For removing: `npm run uninstall`

## Attention! The code in test.cpp was written in a hurry, do not use it in production!

## Build dll:
`g++ -shared -o test.dll test.cpp -lole32 -loleaut32 -lwinmm` \
`lwinmm` here need for `waveInGetNumDevs`-function

Starting the server after installation: \
`npm run start`

After that, on any device on the network, simply open the address of the computer on which you launched the server.

## Known problems
At the moment, there is some kind of error when trying to return a string with a message from the `setVolume` function - the function works, but when returning the value, the Node crashes without any messages. I’m not sure about the correctness of my code, there’s probably some kind of type mismatch, but it’s quite difficult to debug without any-errors — after all, I haven’t written in C for a long time and I can’t immediately understand what’s wrong. When the [ffi-rs](https://github.com/zhangyuang/node-ffi-rs) package is updated and will displays errors, I will try to fix this problem.

For now, you can simply remove returning messages from the `setVolume` and change the function type to void so that the service does not crash when the volume changes.
