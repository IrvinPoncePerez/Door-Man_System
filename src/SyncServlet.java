package door_man;

import java.io.IOException;
import java.util.Calendar;
import java.util.Date;
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
		Date objDate = DateNow.getDateTime();
		Calendar objCalendar = Calendar.getInstance();
		objCalendar.setTime(objDate);
		
		JSONObject objJSON = new JSONObject();
		
		try {
			objJSON.put("year", objCalendar.get(Calendar.YEAR));
			objJSON.put("month", objCalendar.get(Calendar.MONTH) +1);
			objJSON.put("day", objCalendar.get(Calendar.DAY_OF_MONTH));
			objJSON.put("hour", objCalendar.get(Calendar.HOUR_OF_DAY));
			objJSON.put("minute", objCalendar.get(Calendar.MINUTE));
			objJSON.put("second", objCalendar.get(Calendar.SECOND));
					
		} catch (JSONException e) {
			logger.log(Level.WARNING, e.getMessage() + "**" + e.getStackTrace() + "**");
		}
		response.getWriter().print(objJSON.toString());
		logger.log(Level.INFO, "Sync board " + board);
	}
	
}
