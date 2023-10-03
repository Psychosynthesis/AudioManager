const path = require('path');
const Service = require('node-windows').Service;

// Создаем новый объект службы
const svc = new Service({
  name:'NodeJS AudioManager',
  description: 'NodeJS AudioManager as Windows Service',
  script: path.resolve(__dirname)+'\\server.js', // путь к приложению
});

// Слушаем событие 'uninstall', пишем сообщение
svc.on('uninstall', function(){
  console.log('Uninstall complete.');
  console.log('The service exists: ', svc.exists);
});

// Удаляем службу
svc.uninstall();
