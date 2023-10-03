const express = require('express');
const server = express();
const { load, RetType, ParamsType } = require('ffi-rs');
const EventLogger = require('node-windows').EventLogger;
// const dynamicLib = platform === 'win32' ? './test.dll' : "./test.so"

const SERVER_PORT = 8080;
const logger = new EventLogger('New windows-event logger');

// Add headers before the routes are defined
server.use(function (req, res, next) {
  res.setHeader('Access-Control-Allow-Origin', '*'); // Website you wish to allow to connect
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST'); // Request methods you wish to allow
  res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With, content-type'); // Website you wish to allow to connect
  next();
});

server.use(express.json({ limit: '10mb' }));
server.use(express.static(__dirname + '/../public')); // Will return index.html from get http://127.0.0.1:8080/

server.post('/api/setvolume', function(request, response) {
  const input = request.body; // Access the parse results as request.body
  if (!input.hasOwnProperty('volume')) {
    response.status(422);
    response.send('Volume is not specified');
  }
  const newVolume = parseInt(input['volume'], 10) * 0.01;
  if (newVolume < 0 || newVolume > 1) {
    response.status(422);
    response.send('Volume must be in range from 0 to 100');
  }

  try {
    load({
      library: __dirname + '/../test.dll', // path to the dynamic library file
      funcName: 'setVolume', // the name of the function to call
      retType: RetType.String, // the return value type
      paramsType: [ParamsType.Double], // the parameter types
      paramsValue: [newVolume] // the actual parameter values
    });
  } catch(err) {
    logger.error('Something went wrong when tried using extern DLL: ' + err.name + ' ' + err.message, 2);
    console.err(err);
  }

  response.send("OK");
  // console.log("returnValueOfWinApi is: ", returnValueOfWinApi);
});

server.listen(SERVER_PORT, () => {
  logger.info('NodeJS AudioManager server start at port ' + SERVER_PORT, 1); // Second param is event code, use your own
  console.log('Server listening on port ' + SERVER_PORT);
  console.log('Open http://127.0.0.1:'+ SERVER_PORT +' in your browser');
});
