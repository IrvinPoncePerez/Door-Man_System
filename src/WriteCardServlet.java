package door_man;

import java.io.IOException;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.appengine.api.datastore.DatastoreService;
import com.google.appengine.api.datastore.DatastoreServiceFactory;
import com.google.appengine.api.datastore.Entity;



@SuppressWarnings("serial")
public class WriteCardServlet extends HttpServlet{
	
	private static final Logger logger = Logger.getLogger(TokenServlet.class.getCanonicalName());
	private static DatastoreService datastore = DatastoreServiceFactory.getDatastoreService();
	
	protected void doPost(HttpServletRequest request,  HttpServletResponse response) throws ServletException, IOException{
		String function = request.getParameter("function");
		if (function.equals("store")){			
			storingData(request, response);			
		}else if (function == "cancel"){
			cancelData(request, response);
		}else if (function == "write") {
			
		}
	}
	
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException{
		
	}
	
	private void storingData(HttpServletRequest request, HttpServletResponse response) throws IOException{
		try{
			Date today = new Date();
			String doorId = request.getParameter("doorId");
			String userId = request.getParameter("userId");
			String typeCard = request.getParameter("typeCard");
			String forWriter = request.getParameter("forWriter");
		
			Entity card = new Entity("Card");
			
			card.setProperty("DoorId", doorId);
			card.setProperty("UserId", userId);
			card.setProperty("TypeCard", typeCard);
			card.setProperty("DateWrite", String.format("%tD", today));
			card.setProperty("TimeSend", String.format("%tr", today));
			card.setProperty("TimeWrite", String.format("%tr", today));
			card.setProperty("ForWriter", forWriter);
			card.setProperty("IsWrite", false);
			card.setProperty("Active", true);
		
			datastore.put(card);
			
			response.getWriter().print("wait");
		} catch (Exception e){
			logger.log(Level.WARNING, e.getMessage() + "**" + e.getStackTrace() + "**");
			response.getWriter().print("error");
		}
	}
	
	private void cancelData(HttpServletRequest request, HttpServletResponse response){
		
	}
	
}
