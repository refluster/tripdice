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

app.controller('cList', ['$scope', function($scope) {
	$scope.ctx = document.getElementById('virtual-canvas').getContext('2d');
	$scope.thumb = document.getElementById('thumb');

	document.getElementById('file').addEventListener('change', function() {
		var fileReader = new FileReader() ;
		fileReader.onload = function() {
			$scope.thumb.setAttribute('src', this.result);
		}
		fileReader.readAsDataURL(this.files[0]);
	});

	$scope.thumb.onload = function() {
		$scope.ctx.drawImage($scope.thumb, 0, 0, $scope.thumb.width, $scope.thumb.height,
							 0, 0, 320, 240);
		rgb = $scope.ctx.getImageData(0, 0, 320, 240).data;
		var buf = new ArrayBuffer(320*240*2);
		var u16 = new Uint16Array(buf);
		var wIdx = 0;

		for (var x = 0; x < 320; x++) {
			for (var y = 0; y < 240; y++) {
				var index = (y*320 + x)*4;
				var r = rgb[index + 0];
				var g = rgb[index + 1];
				var b = rgb[index + 2];
				var dat = (((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
				u16[wIdx] = dat;
				++wIdx;
			}
		}

		console.log(u16);
		console.log(rgb);
		console.log(rgb.length);
	};
}]);

