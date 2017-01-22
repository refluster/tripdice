const spawn = require('child_process').spawn;
const fs = require('fs');

const lcdcmd = '../../driver/lcd/lcd';

/*
fs.readFile('./test.bin', 'utf8', function (err, data) {
    console.log(data);
});
*/

/*exec('../../driver/lcd/lcd 1 --update -', (err, stdout, stderr) => {
	if (err) { console.log(err); }
	console.log(stdout);
});
*/

//p = spawn('./touch.sh');
//p.stdin.write("hogefuga\n");

p_init = spawn(lcdcmd, ['--init']);
p_init.on('exit', function (code) {
	p_test1 = spawn(lcdcmd, ['0', '--test', '1']);
	p_test1.on('exit', function (code) {
		p_test2 = spawn(lcdcmd, ['1', '--test', '4']);
	});
});
