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
              sessionOpen = true;
              userId = data.id;
              requestToken(userId);
              interval = setInterval(updateTime, 60000);
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

/*
 *  Declaracion de eventos
 */

$('#signout').click(cerrarSesion);


/*
 *Variable global socket
 *
 */
var socket;
var sessionOpen; 
var userId;
var interval;

function updateTime(){
  var timers = $('.infoTiempo');
  for (var i = 0; i < timers.length; i++){
    var tagTime = $(timers[i]);
    var time = parseInt(tagTime.text()) + 1;
    tagTime.text(time);
  }
}

function cargarPuertas(){
  $('#contenido').slideToggle('fast');
  //
  // Aplicador de colores
  //
  $('#aplicadorColores').slideToggle();

  $('#footer').slideToggle('fast');
  $('footer #copyright p:first-child').css('padding-top', '0px');

  var svgPuerta = 'img/puerta.svg';
  
  $.ajax({
    url: 'json/puertas.json',
    dataType: 'json',
    success: function(data){
      $('#contenidoPuertas').append('<section id="seccionPuertas"></section>');
      for (var i = 0; i < data.puertas.length; i++){

        var habitacion = '<article id="' + data.puertas[i].id + '" class="contPuerta">' +
                            '<p class="tituloPuerta">' + data.puertas[i].puerta + '</p>' +
                            '<div class="contDatos">' +
                              '<figure><object id="svg' + data.puertas[i].id + '" type="image/svg+xml" data="' + svgPuerta + '"></object></figure>' +
                              '<div class="contInfo">' +
                                '<p class="infoBateria"></p>' +
                                '<p class="infoTiempo"></p>' +
                                '<p class="infoEntrada"></p>' +
                                '<p class="infoSalida"></p>' +
                                '<p class="infoDescripcion"></p>' +
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
        $('#footer').slideToggle('fast');
        $('#contenido').slideToggle('fast');
        //
        // Aplicador de colores
        //
        $('#nombreUsuario').remove();
        $('#avatar img').remove();
        $('#seccionPuertas').remove();
        sessionOpen = false;
        socket.close();
        clearInterval(interval);
      });
    },
    error : function(e) {
      alert('Error producido al cerrar su sesión, si desea cerrar su sesión de forma manual puede hacerlo en https://plus.google.com/apps.');
    }
  });
}

// Puerta disponible : #9CBF60
// Puerta ocupada : #D34B44
// Puerta mantenimiento : #DEAA31
// Puerta limpieza : #85C7C3
// Puerta sucia : #A89565   
    
// fetches the document for the given embedding_element
function getSubDocument(embedding_element)
{
  if (embedding_element.contentDocument) 
  {
    return embedding_element.contentDocument;
  } 
  else 
  {
    var subdoc = null;
    try {
      subdoc = embedding_element.getSVGDocument();
    } catch(e) {}
    return subdoc;
  }
}
    
function findSVGElements(puerta, color)
{
  $('#' + puerta + ' .contDatos figure').css('border-color', color);
  var svgObject = document.getElementById('svg' + puerta);
  var svgDocument = getSubDocument(svgObject);
  
  if (svgDocument){
    var fondos = svgDocument.getElementsByClassName('fondo');
    var marcos = svgDocument.getElementsByClassName('marco')
      for (var j = 0; j < fondos.length; j++){
        fondos[j].setAttribute('fill', color);
      }
      for (var j = 0; j < marcos.length; j++){
        marcos[j].setAttribute('stroke', color);
      }
  }
}

function setPuerta(puerta){
  var idPuerta = puerta.id;

  $('#'+ idPuerta + ' .contDatos .contInfo .infoBateria').html(puerta.bateria);
  $('#'+ idPuerta + ' .contDatos .contInfo .infoTiempo').html(puerta.tiempo);
  $('#'+ idPuerta + ' .contDatos .contInfo .infoEntrada').html(puerta.entrada);
  $('#'+ idPuerta + ' .contDatos .contInfo .infoSalida').html(puerta.salida);
  $('#'+ idPuerta + ' .contDatos .contInfo .infoDescripcion').html(puerta.descripcion);
  findSVGElements(puerta.id, puerta.estatus);
}

/*
 *  Código para realizar la conexión con Socket al 
 *  servidor App Engine.
 *
 */

requestToken = function( userId ) {
  var getTokenURI = '/gettoken?userId=' + userId;

  $.ajax({
    type: 'POST',
    url: getTokenURI,
    async: true,
    contentType: 'text/plain',
    data: 'text',
    success: openChannel
  });

};

openChannel = function ( token ) {
  var channel = new goog.appengine.Channel(token);
  socket = channel.open();

  socket.onopen = onSocketOpen;
  socket.onmessage = onSocketMessage;
  socket.onerror = onSocketError;
  socket.onclose = onSocketClose;
};

onSocketError = function ( error ) {
  alert('Error al abrir la conexión');
};

onSocketOpen = function () {
  $.ajax({
    url: 'json/estatusPuertas.json',
    dataType: 'json',
    success: function(data){
      for (var i = 0; i < data.puertas.length; i++){
        setPuerta(data.puertas[i]);
      }
    }
  });
};

onSocketClose = function () {
  if (sessionOpen) {
    requestToken(userId);
  }
};

onSocketMessage = function (message) {
  var data = JSON.parse(message.data);
  if (data.type == 'updateEstatus'){
    updateEstatus(data);
  }
};

/*
 * Funciones para la atualizacion de los mensajes recibidos.
 *
 */

function updateEstatus(puerta){
  var idPuerta = puerta.id;

  $('#' + idPuerta + ' .contDatos .contInfo .infoBateria').html(puerta.bateria);
  if (puerta.actividad == 'insideOpen') {
    $('#' + idPuerta + ' .contDatos .contInfo .infoSalida').html(puerta.hora);
    $('#' + idPuerta + ' .contDatos figure object').addClass(puerta.actividad)
  } else if (puerta.actividad == 'outsideOpen') {
    $('#' + idPuerta + ' .contDatos .contInfo .infoEntrada').html(puerta.hora);
    $('#' + idPuerta + ' .contDatos figure object').addClass(puerta.actividad)
  } else if (puerta.actividad == 'closed') {
    if ($('#' + idPuerta + ' .contDatos figure object').hasClass('insideOpen')) {
      $('#' + idPuerta + ' .contDatos figure object').removeClass('insideOpen')
    } 
    if ($('#' + idPuerta + ' .contDatos figure object').hasClass('outsideOpen')) {
      $('#' + idPuerta + ' .contDatos figure object').removeClass('outsideOpen')
    }

  }  
}