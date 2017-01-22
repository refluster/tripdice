const spawn = require('child_process').spawn;

const lcdcmd = '../driver/lcd/lcd';

/*exec('../../driver/lcd/lcd 1 --update -', (err, stdout, stderr) => {
	if (err) { console.log(err); }
	console.log(stdout);
});
*/

//p = spawn('./touch.sh');
//p.stdin.write("hogefuga\n");

/*
p_init = spawn(lcdcmd, ['--init']);
p_init.on('exit', function (code) {
	p_test1 = spawn(lcdcmd, ['0', '--test', '1']);
	p_test1.on('exit', function (code) {
		fs.readFile('./test.bin', function (err, data) {
			p_test2 = spawn(lcdcmd, ['1', '--update', '-']);
			p_test2.stdin.write(data);
		});
	});
});
*/

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
