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

exports.EV_SYN = binding.EV_SYN;
exports.EV_KEY = binding.EV_KEY;
exports.EV_REL = binding.EV_REL;
exports.EV_ABS = binding.EV_ABS;
exports.EV_MSC = binding.EV_MSC;
exports.EV_SW  = binding.EV_SW;
exports.EV_LED = binding.EV_LED;
exports.EV_SND = binding.EV_SND;
exports.EV_REP = binding.EV_REP;
exports.EV_FF  = binding.EV_FF;
exports.EV_PWR = binding.EV_PWR;
exports.EV_FF_STATUS = binding.EV_FF_STATUS;
exports.EV_MAX = binding.EV_MAX;
exports.EV_CNT = binding.EV_CNT;


exports.BTN_EXTRA = binding.BTN_EXTRA;
exports.BTN_0     = binding.BTN_0;

