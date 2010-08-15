var fs  = require('fs'),
    sys = require('sys'),
    events = require('events');

var binding = require('./binding');

exports.createInputEventSource = function(device_path) {
  var src = new binding.Source();
  src.event_stream = fs.createReadStream(device_path, {flags: 'r'});

  src.event_stream.on('data', function(data) {
    src.dispatchEventsInBuffer(data);
  });
  return src;
};
