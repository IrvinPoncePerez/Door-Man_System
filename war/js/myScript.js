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
              var userName = data.displayName;
              var imgUser = data.image.url;
              var urlUser = imgUser.slice(0, -2);
              var picAvatar = new Image();
              picAvatar.src = urlUser + '200';

              $('header #user .data_user').prepend('<p id="user_name">' + userName + '</p>');
              $('header #user #avatar').prepend(picAvatar);
              
              $('#login').slideToggle('fast', function(){
                $('#user').slideToggle('fast');
              });

              $('#avatar img').click(function() {
                if ($(window).width() <= 680){
                  if ($('header #user div').css('display') === 'none'){
                    $('header #user div').css('display', 'inline-block');
                  } else {
                    $('header #user div').css('display', 'none');
                  }                  
                }
              });
              sessionOpen = true;
              userId = data.id;
              name = data.displayName;
              requestToken();
              interval = setInterval(updateTime, 60000);
            },
            error: function(e) {
              alert(e);
            }
          });
          loadDoors();
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

$('#signout').click(closeSession);


/*
 *Variable global socket
 *
 */
var socket;
var sessionOpen; 
var userId;
var name;
var interval;
var doorId;

function updateTime(){
  var timers = $('.info_time');
  for (var i = 0; i < timers.length; i++){
    var tagTime = $(timers[i]);
    var time = parseInt(tagTime.text()) + 1;
    tagTime.text(time);
  }
}

function loadDoors(){
  $('#content_presentation').slideToggle('fast');
  $('#footer').slideToggle('fast');
  $('footer #copyright p:first-child').css('padding-top', '0px');

  var svgDoor = 'img/puerta.svg';
  
  $.ajax({
    url: 'json/puertas.json',
    dataType: 'json',
    success: function(data){
      $('#content_doors').append('<section id="section_doors"></section>');
      for (var i = 0; i < data.doors.length; i++){

        var room = '<article id="' + data.doors[i].id + '" class="content_door">' +
                            '<p class="title_door">' + data.doors[i].door + '</p>' +
                            '<div class="content_info">' +
                              '<figure onClick="setRoom(' + data.doors[i].id + ')"><object id="svg' + data.doors[i].id + '" type="image/svg+xml" data="' + svgDoor + '"></object></figure>' +
                              '<div class="info">' +
                                '<p class="info_time"></p>' +
                                '<p class="info_input"></p>' +
                                '<p class="info_output"></p>' +
                                '<p class="info_description"></p>' +
                              '</div>' +
                            '</div>' +
                          '</article>';

        $('#section_doors').append(room);
      }
    }    
  });
}

function setRoom(data){
  var color = $(data).find('figure').css('border-color');
  doorId = $(data).attr('id');
  if (color == 'rgb(156, 191, 96)' && userId == '115380400632455375055'){ //Disponible

    $.ajax({
      type: 'GET',
      url: 'html/escritura.html',
      async :false,
      success: function(dialog){

        var title = $(data).find('.title_door').text();

        $('#open_modal').append(dialog);        
        $('#title_room').text(title);
        $('#open_modal').addClass('show');        
        $('#dialog_write .close').click(function(){
          $('.modal_dialog').removeClass('show');
          $('#dialog_write').remove();
        });

        var radioButtons = $('#dialog_write form fieldset input[type="radio"]')
        for (var i = 0; i < radioButtons.length; i++){
          $(radioButtons[i]).change(function(){
            var valRadio = $(this).val();
            var sendButton = $('#send_button');
            var textStatus = "Esperando el envió de datos tipo " + valRadio + "...";
            $('#status_dialog').text(textStatus);
            if (sendButton.css('display') == 'none'){
              sendButton.addClass('show');
            }
          });
        }
      }
    });
    
  } else if (color == 'rgb(211, 75, 68)') { //Ocupada

  } else if (color == 'rgb(222, 170, 49)') { //Mantenimiento

  } else if (color == 'rgb(168, 149, 101)') { //Supervisor

  } else if (color == 'rgb(133, 199, 195)') { //Limpieza

  }
}

function sendDialogWrite(){
  var textButton = $('#send_button').text();
  
  if (textButton == 'Enviar'){
    sendDataWrite();
  } else if (textButton == 'Cancelar'){
    cancelDataWrite();
  } else if (textButton == 'Cerrar'){
    $('.modal_dialog').removeClass('show');
    $('#dialog_write').remove();
  }

  return false;
}

function cancelDataWrite(){
  $.ajax({
    type: 'POST',
    url: '/writecard?function=cancel&userId=' + userId + '&doorId=' + doorId,
    async: false,
    success: function(data){
      if (data == 'cancel'){
        $('.modal_dialog').removeClass('show');
        $('#dialog_write').remove();
      }
    }
  });
}

function sendDataWrite(){
  var type_user = $('#dialog_write form fieldset input[type="radio"]:checked').val();
   $.ajax({
    type : 'POST',
    url : '/writecard?function=store&doorId=' + doorId + '&userId=' + userId + '&card=' + type_user + '&writer=Arduino1',
    async : false,
    success : function(data){
      if (data == 'wait'){
        $('#status_dialog').text('Coloque la tarjeta a escribir, esperando...');
        $('#dialog_write .close').css('display', 'none');
        $('#send_button').text('Cancelar');
        $('#send_button').addClass('cancel');
      }else if (data == 'error'){
        alert("Error al almacenar los datos de escritura.");
      }
    }
   }); 
}

function closeSession(access_token) {
  var revokeUrl = 'https://accounts.google.com/o/oauth2/revoke?token=' + access_token;

  $.ajax({
    type : 'GET',
    url: revokeUrl,
    async : false,
    contentType : 'appication/json',
    dataType : 'jsonp',
    success : function(){
        $('#user').slideToggle('fast', function(){
        $('#login').slideToggle('fast');
        $('#footer').slideToggle('fast');
        $('#content_presentation').slideToggle('fast');
        //
        // Aplicador de colores
        //
        $('#user_name').remove();
        $('#avatar img').remove();
        $('#section_doors').remove();
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

// door disponible : #9CBF60
// door cliente : #D34B44
// door mantenimiento : #DEAA31
// door limpieza : #85C7C3
// door supervisor : #A89565   
    
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
    
function findSVGElements(door, color)
{
  $('#' + door + ' .content_info figure').css('border-color', color);
  var svgObject = document.getElementById('svg' + door);
  var svgDocument = getSubDocument(svgObject);
  
  if (svgDocument){
    var fills = svgDocument.getElementsByClassName('fondo');
    var strokes = svgDocument.getElementsByClassName('marco')
      for (var j = 0; j < fills.length; j++){
        fills[j].setAttribute('fill', color);
      }
      for (var j = 0; j < strokes.length; j++){
        strokes[j].setAttribute('stroke', color);
      }
  }

  if (color == '#9CBF60'){
    $('#' + door + ' .content_info figure').css('cursor', 'pointer');
  }
}

function setDoor(door){
  var idDoor = door.id;

  $('#'+ idDoor + ' .content_info .info .info_time').html(door.time);
  $('#'+ idDoor + ' .content_info .info .info_input').html(door.input);
  $('#'+ idDoor + ' .content_info .info .info_output').html(door.output);
  $('#'+ idDoor + ' .content_info .info .info_description').html(door.description);
  findSVGElements(door.id, door.status);
}

/*
 *  Código para realizar la conexión con Socket al 
 *  servidor App Engine.
 *
 */

requestToken = function() {
  var getTokenURI = '/gettoken?userId=' + userId + '&name=' + name;

  $.ajax({
    type: 'POST',
    url: getTokenURI,
    async: true,
    contentType: 'text/plain',
    data: 'text',
    success: openChannel
  });

};

openChannel = function (token) {
  var channel = new goog.appengine.Channel(token);
  socket = channel.open();

  socket.onopen = onSocketOpen;
  socket.onmessage = onSocketMessage;
  socket.onerror = onSocketError;
  socket.onclose = onSocketClose;
};

onSocketError = function (error) {
  alert('Error al abrir la conexión');
};

onSocketOpen = function () {
  $.ajax({
    url: 'json/estatusPuertas.json',
    dataType: 'json',
    success: function(data){
      for (var i = 0; i < data.doors.length; i++){
        setDoor(data.doors[i]);
      }
    }
  });
};

onSocketClose = function () {
  if (sessionOpen) {
    requestToken();
  }
};

onSocketMessage = function (message) {
  var data = JSON.parse(message.data);
  if (data.type == 'update_status'){
    updateStatus(data);
  } else if (data.type == 'writed'){
    $('#status_dialog').text('Tarjeta escrita correctamente...');
    $('#send_button').text('Cerrar');
    $('#send_button').removeClass('cancel');
  }
};

/*
 * Funciones para la atualizacion de los mensajes recibidos.
 *
 */

function updateStatus(door){

  //Actualización de los datos cuando sea abierto por dentro
  if (door.activity == 'inside_open') {

    $('#' + door.id + ' .content_info .info .info_output').html(door.hour);
    $('#' + door.id + ' .content_info figure object').addClass(door.activity);

  //Actualización de los datos cuando sea abierto por fuera
  } else if (door.activity == 'outside_open') {
    
    $('#' + door.id + ' .content_info .info .info_input').html(door.hour);
    $('#' + door.id + ' .content_info figure object').addClass(door.activity);

  //actualización de la animación cuando la puerta sea cerrada
  } else if (door.activity == 'closed') {
    
    if ($('#' + door.id + ' .content_info figure object').hasClass('inside_open')) {
    
      $('#' + door.id + ' .content_info figure object').removeClass('inside_open');

    } 
    if ($('#' + door.id + ' .content_info figure object').hasClass('outside_open')) {
      
      $('#' + door.id + ' .content_info figure object').removeClass('outside_open');

    }

  }  

  // door disponible : #9CBF60
  // door cliente : #D34B44
  // door mantenimiento : #DEAA31
  // door limpieza : #85C7C3
  // door supervisor : #A89565   
  if (door.card != null){
    if (door.card == "Disponible"){
      findSVGElements(door.id, '#9CBF60');
    } else if (door.card == "Cliente"){
      findSVGElements(door.id, '#D34B44');
    } else if (door.card == "Limpieza"){
      findSVGElements(door.id, '#85C7C3');
    } else if (door.card == "Supervisor"){
      findSVGElements(door.id, '#A89565 ');
    } else if (door.card == "Mantenimiento"){
      findSVGElements(door.id, '#DEAA31');
    }
  }
}