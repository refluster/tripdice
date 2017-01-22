const spawn = require('child_process').spawn;

const lcdcmd = '../driver/lcd/lcd';

exports.init = function(callback) {
	var p = spawn(lcdcmd, ['--init']);
	p.on('exit', function(code) {
		if (callback !== undefined) {
			callback();
		}
	});
};

exports.update = function(lcd_no, data, callback) {
	var p = spawn(lcdcmd, ['1', '--update', '-']);
	p.stdin.write(data);
	p.on('exit', function(code) {
		if (callback !== undefined) {
			callback();
		}
	});
};
