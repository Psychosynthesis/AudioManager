const path = require('path');
const Service = require('node-windows').Service;

// Создаем новый объект службы
const svc = new Service({
  name:'NodeJS AudioManager',
  description: 'NodeJS AudioManager as Windows Service',
  script: path.resolve(__dirname)+'\\server.js', // путь к приложению
  maxRetries: 10, // не перезапускать службу после 10 падений (защита от цикла)
});

// Слушаем событие 'install' и запускаем службу
svc.on('install',function(){
  svc.start();
});

// Устанавливаем службу
svc.install();
