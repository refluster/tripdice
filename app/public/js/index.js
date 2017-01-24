app = angular.module('App', ['ngRoute']);

app.config(['$routeProvider', function ($routeProvider) {
	$routeProvider
		.when('/', {
			templateUrl: 'index-tmpl',
			controller: 'cList'
		})
        .otherwise({
            redirectTo: '/'
        });
}]);

app.controller('cMain', ['$scope', function($scope) {
}]);

app.controller('cList', ['$scope', '$http', function($scope, $http) {
	$scope.ctx = document.getElementById('virtual-canvas').getContext('2d');
	$scope.thumb = document.getElementById('image-thumbnail1');

	// trigger to update image
	Array.prototype.forEach.call(document.getElementsByClassName('image-selector'), function(elem) {
		elem.addEventListener('change', function(e) {
			var fileReader = new FileReader() ;
			fileReader.onload = function() {
				$scope.thumb.setAttribute('src', this.result);
			}
			fileReader.readAsDataURL(this.files[0]);
		});
	}.bind(this));

	$scope.thumb.onload = function() {
		$scope.ctx.drawImage($scope.thumb,
							 0, 0, $scope.thumb.naturalWidth, $scope.thumb.naturalHeight,
							 0, 0, 320, 240);
		rgba = $scope.ctx.getImageData(0, 0, 320, 240).data;
		var u16 = new Uint16Array(new ArrayBuffer(320*240*2));

		// convert from 32bit rgba to 16bit rgb image
		var wIdx = 0;
		for (var x = 0; x < 320; x++) {
			for (var y = 0; y < 240; y++) {
				var index = (y*320 + x)*4;
				var r = rgba[index + 0];
				var g = rgba[index + 1];
				var b = rgba[index + 2];
				var dat = (((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
				u16[wIdx] = dat;
				++wIdx;
			}
		}

		// Uint16Array -> Uint8Array -> base64
		var b64 = btoa(Array.from(new Uint8Array(u16.buffer),
								  e => String.fromCharCode(e)).join(''));

		console.log(b64.length);

		$http({
			method : 'POST',
			url : 'api',
			data: {method: 'update', lcd_no: 1, b64: b64}
		}).then(function mySucces(response) {
			console.log('succeed');
			console.log(response.data);
		}, function myError(response) {
			console.log('failed');
			console.log(response.statusText);
		});

		console.log(u16);
		console.log(rgba);
		console.log(rgba.length);
	};
}]);

