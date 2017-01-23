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
	};
}]);

