package door_man;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.appengine.api.channel.ChannelFailureException;
import com.google.appengine.api.channel.ChannelMessage;
import com.google.appengine.api.channel.ChannelService;
import com.google.appengine.api.channel.ChannelServiceFactory;
import com.google.appengine.labs.repackaged.org.json.JSONException;
import com.google.appengine.labs.repackaged.org.json.JSONObject;

import door_man.EstatusServlet;

@SuppressWarnings("serial")
public class EstatusServlet extends HttpServlet{
	
	private static final Logger logger = Logger.getLogger(EstatusServlet.class.getCanonicalName());		  
	private static ChannelService channelService = ChannelServiceFactory.getChannelService();
	
	
	
	/**
	 * Crea el mensaje del canal y lo envia al cliente del canal.
	 * @param user
	 * @param estatus
	 */
	public void sendEstatusToChannel(String user, String estatus) throws ChannelFailureException {
		channelService.sendMessage(new ChannelMessage(user, estatus));
	}
	
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		JSONObject objJSON = new JSONObject();
		try {
			
			objJSON.put("type", "updateEstatus");
			objJSON.put("id", request.getParameter("puerta"));
			objJSON.put("bateria", request.getParameter("bateria"));
			objJSON.put("actividad", request.getParameter("actividad"));
			objJSON.put("hora", request.getParameter("hora"));
			
			logger.log(Level.INFO, "sending estatus info to the channel.");
			sendEstatusToChannel("115380400632455375055", objJSON.toString());
		} catch (ChannelFailureException ex) {
			logger.log(Level.WARNING, "Failed in sending message to channel");
			//borrar el usuario.
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}

}
