function signinCallback (authResult) {
	if (authResult['access_token']) {
		//Autorizado correctamente
		//Oculta el boton de inicio de sesion ahora que el usuario está autorizado:
		//document.getElementById('customBtn').setAttribute('style', 'display: none');
		console.log(authResult);
		$.get('https://www.googleapis.com/plus/v1/people/me', function(data){
			console.log(data);
		});
	} else {
		//en caso de error, avisar al usuario :
		if (authResult['access_denied']) {
			alert("El usuario ha denegado el acceso a la aplicación.");
		}
		if (authResult['immediate_failed']) {
			alert("No se ha podido dar acceso al usuario de forma automática.");
		}		
	}
}
