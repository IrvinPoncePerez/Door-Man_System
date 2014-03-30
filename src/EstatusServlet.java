package door_man;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.appengine.api.datastore.Entity;

import com.google.appengine.labs.repackaged.org.json.JSONException;
import com.google.appengine.labs.repackaged.org.json.JSONObject;

import door_man.EstatusServlet;

@SuppressWarnings("serial")
public class EstatusServlet extends HttpServlet{
	
	private static final Logger logger = Logger.getLogger(EstatusServlet.class.getCanonicalName());		  
		
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		JSONObject objJSON = new JSONObject();
		UsersStore objUsersStore = UsersStore.getInstance();
		try {
			
			objJSON.put("type", "update_status");
			objJSON.put("id", request.getParameter("door"));
			objJSON.put("battery", request.getParameter("battery"));
			objJSON.put("activity", request.getParameter("activity"));
			objJSON.put("hour", request.getParameter("hour"));
					
			 for (Entity user : objUsersStore.getUsers()){
				 logger.log(Level.INFO, "sending estatus info to the channel " + user);
				 ChannelServlet.sendMessage(user.getProperty("UserId").toString(), objJSON.toString());
			 }
			
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}

}
