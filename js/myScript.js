(function() {
       var po = document.createElement('script'); 
       po.type = 'text/javascript'; 
       po.async = true;
       po.src = 'https://apis.google.com/js/client:plusone.js?onload=render';
       var s = document.getElementsByTagName('script')[0]; 
       s.parentNode.insertBefore(po, s);
})();

function render() {
	gapi.signin.render('login', {
		'callback' : 'signinCallback',
		'clientid' : '383828807826-od8raqqomos3vbkseoph3uec76b7ucd1.apps.googleusercontent.com',
		'cookiepolicy': 'single_host_origin',
      	'requestvisibleactions': 'http://schemas.google.com/AddActivity',
      	'scope': 'https://www.googleapis.com/auth/plus.login'
	});
}

function signinCallback (authResult) {
      if (authResult['access_token']) {
            //Autorizado correctamente
            //document.getElementById('customBtn').setAttribute('style', 'display: none');
          var token = gapi.auth.getToken();
          var accessToken = token.access_token;
          var accessUrl = 'https://www.googleapis.com/plus/v1/people/me?access_token=' + accessToken;
          $.ajax({
            type: 'GET',
            url: accessUrl,
            async:false,
            contentType : 'appication/json',
            dataType : 'jsonp',
            success: function(data) {
              var nombreUsuario = data.displayName;
              var imgUsuario = data.image.url;
              var urlUsuario = imgUsuario.slice(0, -2);
              var picAvatar = new Image();
              picAvatar.src = urlUsuario + '200';

              $('header #usuario .datos').prepend('<p id="nombreUsuario">' + nombreUsuario + '</p>');
              $('header #usuario #avatar').prepend(picAvatar);
              
              $('#login').slideToggle('fast', function(){
                $('#usuario').slideToggle('fast');
              });

              $('#avatar img').click(function() {
                if ($(window).width() <= 680){
                  if ($('header #usuario div').css('display') === 'none'){
                    $('header #usuario div').css('display', 'inline-block');
                  } else {
                    $('header #usuario div').css('display', 'none');
                  }                  
                }
              });
              
            },
            error: function(e) {
              alert(e);
            }
          });
          cargarPuertas();
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

function cargarPuertas(){
  $('#contenido').slideToggle('fast');
  $('footer').slideToggle('fast');
  var svgPuerta = null

  if ($(window).width() <= 680){
    svgPuerta = '<object data="img/chapa.svg"></object>';
  } else {
    svgPuerta = '<object data="img/puerta.svg"></object>';
  }
  
  $.ajax({
    url: 'json/puertas.json',
    dataType: 'json',
    success: function(data){
      $('#contenidoPuertas').append('<section id="seccionPuertas"></section>');
      for (var i = 0; i < data.puertas.length; i++){
        var habitacion = '<article id="puerta' + (i + 1)+ '" class="contPuerta">' +
                            '<p class="tituloPuerta">' + data.puertas[i].puerta + '</p>' +
                            '<div class="contDatos">' +
                              '<figure>' + svgPuerta + '</figure>' +
                              '<div class="contInfo">' +
                                '<p class="infoBateria">' + data.puertas[i].bateria +'</p>' +
                                '<p class="infoTiempo">' + data.puertas[i].tiempo +'</p>' +
                                '<p class="infoEntrada">' + data.puertas[i].entrada +'</p>' +
                                '<p class="infoSalida">' + data.puertas[i].salida +'</p>' +
                                '<p class="infoDescripcion">' + data.puertas[i].Descripcion +'</p>' +
                              '</div>' +
                            '</div>' +
                          '</article>';

        $('#seccionPuertas').append(habitacion);
      }
    }    
  });
}

function cerrarSesion(access_token) {
  var revokeUrl = 'https://accounts.google.com/o/oauth2/revoke?token=' + access_token;

  $.ajax({
    type : 'GET',
    url: revokeUrl,
    async : false,
    contentType : 'appication/json',
    dataType : 'jsonp',
    success : function(){
      $('#usuario').slideToggle('fast', function(){
        $('#login').slideToggle('fast');
        $('footer').slideToggle('fast');
        $('#contenido').slideToggle('fast');
        $('#nombreUsuario').remove();
        $('#avatar img').remove();
      });
    },
    error : function(e) {
      alert('Error producido al cerrar su sesión, si desea cerrar su sesión de forma manual puede hacerlo en https://plus.google.com/apps.');
    }
  });
}

//Declaracion de eventos
$('#signout').click(cerrarSesion);