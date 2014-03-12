package door_man;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.appengine.labs.repackaged.org.json.JSONException;
import com.google.appengine.labs.repackaged.org.json.JSONObject;

@SuppressWarnings("serial")
public class SyncServlet extends HttpServlet{
	
	private static final Logger logger = Logger.getLogger(EstatusServlet.class.getCanonicalName());

	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException{
		String board = request.getParameter("board");
		Long time =  DateNow.getLongTime();
		
		JSONObject objJSON = new JSONObject();
		
		try {
			objJSON.put("time", time.toString());
		} catch (JSONException e) {
			logger.log(Level.WARNING, e.getMessage() + "**" + e.getStackTrace() + "**");
		}
		
		response.getWriter().print(objJSON.toString());
		logger.log(Level.INFO, "Sync board " + board);
	}
	
}
