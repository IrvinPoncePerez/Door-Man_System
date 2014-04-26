package door_man;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.jdo.annotations.Serialized;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.appengine.api.datastore.Entity;
import com.google.appengine.labs.repackaged.org.json.JSONException;
import com.google.appengine.labs.repackaged.org.json.JSONObject;
import com.google.apphosting.utils.servlet.ParseBlobUploadFilter;

import door_man.EstatusServlet;

@SuppressWarnings("serial")
public class EstatusServlet extends HttpServlet{
	
	private static final Logger logger = Logger.getLogger(EstatusServlet.class.getCanonicalName());		  
		
	@Override
	@Serialized
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		JSONObject objJSON = new JSONObject();
		UsersStore objUsersStore = UsersStore.getInstance();
		try {
			String params[] = {"", "", "", ""};
			String status = request.getParameter("status");
			char data[] = status.toCharArray();
			
			int j = 0;
			for (int i = 0; i < params.length; i++){
				for (j = j; j < status.length(); j++){
					if (data[j] == '/'){
						j++;
						break;
					} else {
						params[i] = params[i] + data[j];
					}
				}
			}
			
			objJSON.put("type", "update_status");
			objJSON.put("id", params[0]);
			objJSON.put("activity", params[1]);
			objJSON.put("card", params[2]);
			objJSON.put("hour", params[3]);
					
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
