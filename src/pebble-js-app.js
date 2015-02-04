  //  file: pebble-js-app.js
//  auth: Matthew Clark, SetPebble

// change this token for your project
var setPebbleToken = 'Q9BH';


Pebble.addEventListener('ready', function(e) {
  console.log('ready');
});

Pebble.addEventListener('appmessage', function(e) {
  key = e.payload.action;
  console.log('AppMessage received: ' + e.payload.action);
  if (typeof(key) != 'undefined') {
    var settings = localStorage.getItem(setPebbleToken);
    if (typeof(settings) == 'string') {
      try {
        Pebble.sendAppMessage(JSON.parse(settings));
      } catch (e) {
        console.log('appmessage exception');
      }
    }
    var request = new XMLHttpRequest();
    request.open('GET', 'http://mfutech.github.io/PF_HeureEnFrancais/settings.html', true);
    request.onload = function(e) {
      if (request.readyState == 4)
        if (request.status == 200)
          try {
            Pebble.sendAppMessage(JSON.parse(request.responseText));
          } catch (e) {
            console.log('Exception Raised');
          }
    }
    request.send(null);
  }
});

Pebble.addEventListener('showConfiguration', function(e) {
  console.log('Account Token: ' + Pebble.getAccountToken());
  var config = localStorage.getItem(setPebbleToken);
  Pebble.openURL('http://mfutech.github.io/PF_HeureEnFrancais/settings.html?'+config);
});

Pebble.addEventListener('webviewclosed', function(e) {
  if ((typeof(e.response) == 'string') && (e.response.length > 0)) {
    try {
      var response = JSON.parse(e.response);
      console.log('Response: ' + JSON.stringify(response));
      Pebble.sendAppMessage(response);
      localStorage.setItem(setPebbleToken, e.response);
    } catch(e) {
      console.log('webviewclosed exception');
    }
  }
});