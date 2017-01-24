var App = function() {
	this.express = require('express');
	this.bodyParser = require('body-parser');
	this.http = require('http');
	this.path = require('path');
	this.fs = require('fs');
	
	this.app = this.express();
	this.server;
};

App.prototype.stop = function() {
    // close http server
    this.server.close();
}

App.prototype.start = function() {
	// lcd
	this.lcd = require('./lib/lcd');
	this.lcd.init(function() {
		this.fs.readFile('./test.bin', function (err, data) {
			console.log('update')
			this.lcd.update('1', data);
		}.bind(this));
	}.bind(this));

	// server settings
	this.app.set('port', 80);
	this.app.set('view engine', 'ejs');
	this.app.use(this.express.static(this.path.join(__dirname, 'public')));

	this.app.use(this.bodyParser.urlencoded({extended: true}));
	this.app.use(this.bodyParser.json());

	// start server
	this.server = this.http.createServer(this.app);
	this.server.listen(this.app.get('port'), function() {
		console.log('Express server listening on port ' + this.app.get('port'));
	}.bind(this));

	this.app.post('/api', function(req, res) {
		console.log('api received');
		console.log(req.body);
		res.send('ok');
	});
}

app = new App();
app.start();
