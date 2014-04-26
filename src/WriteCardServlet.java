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
import com.google.appengine.api.datastore.PreparedQuery;
import com.google.appengine.api.datastore.Query;
import com.google.appengine.api.datastore.Query.Filter;
import com.google.appengine.api.datastore.Query.FilterOperator;
import com.google.appengine.api.datastore.Query.CompositeFilterOperator;
import com.google.appengine.api.datastore.Query.FilterPredicate;
import com.google.appengine.labs.repackaged.org.json.JSONException;
import com.google.appengine.labs.repackaged.org.json.JSONObject;


@SuppressWarnings("serial")
public class WriteCardServlet extends HttpServlet{
	
	private static final Logger logger = Logger.getLogger(ChannelServlet.class.getCanonicalName());
	private static DatastoreService datastore = DatastoreServiceFactory.getDatastoreService();
	
	protected void doPost(HttpServletRequest request,  HttpServletResponse response) throws ServletException, IOException{
		String function = request.getParameter("function");
		if (function.equals("store")){			
			storingData(request, response);			
		}else if (function.equals("cancel")){
			cancelData(request, response);
		}else if (function.equals("write")) {
			confirm(request);
		}
	}

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException{
		String writer = request.getParameter("writer");
		
		Filter writerFilter = new FilterPredicate("Writer", FilterOperator.EQUAL, writer);
		Filter activeFilter = new FilterPredicate("Active", FilterOperator.EQUAL, true);
		Filter validFilter = CompositeFilterOperator.and(writerFilter, activeFilter);
		
		Query objQuery = new Query("Card").setFilter(validFilter);
		PreparedQuery objPreparedQuery = datastore.prepare(objQuery);
		Entity card = objPreparedQuery.asSingleEntity();
		if (card != null){
			JSONObject objJsonObject = new JSONObject();
			try{
				objJsonObject.put("doorId", card.getProperty("DoorId"));
				objJsonObject.put("card", card.getProperty("TypeCard"));
				objJsonObject.put("userId", card.getProperty("UserId"));
			} catch (JSONException e){
				logger.log(Level.WARNING, e.getMessage() + "**" + e.getStackTrace() + "**");
			}
			response.getWriter().print(objJsonObject.toString());
			logger.log(Level.INFO, "Write Card for " + card.getProperty("DoorId"));
		} else {
			logger.log(Level.INFO, "Card Null! " + writer);
		}
		
	}
	
	private void storingData(HttpServletRequest request, HttpServletResponse response) throws IOException{
		try{
			Date today = DateNow.getDateTime();
			String doorId = request.getParameter("doorId");
			String userId = request.getParameter("userId");
			String typeCard = request.getParameter("card");
			String writer = request.getParameter("writer");
		
			Entity card = new Entity("Card");
			
			card.setProperty("DoorId", doorId);
			card.setProperty("UserId", userId);
			card.setProperty("TypeCard", typeCard);
			card.setProperty("DateWrite", String.format("%tD", today));
			card.setProperty("TimeSend", String.format("%tr", today));
			card.setProperty("TimeWrite", String.format("%tr", today));
			card.setProperty("Writer", writer);
			card.setProperty("IsWrite", false);
			card.setProperty("Active", true);
		
			datastore.put(card);
			
			response.getWriter().print("wait");
		} catch (Exception e){
			logger.log(Level.WARNING, e.getMessage() + "**" + e.getStackTrace() + "**");
			response.getWriter().print("error");
		}
	}
	
	private void cancelData(HttpServletRequest request, HttpServletResponse response) throws IOException {
		
		String userId = request.getParameter("userId");
		String doorId = request.getParameter("doorId");
		
		Filter doorFilter = new FilterPredicate("DoorId", FilterOperator.EQUAL, doorId);
		Filter userFilter = new FilterPredicate("UserId", FilterOperator.EQUAL, userId);
		Filter activeFilter = new FilterPredicate("Active", FilterOperator.EQUAL, true);
		Filter validFilter = CompositeFilterOperator.and(doorFilter, userFilter, activeFilter);
		
		Query objQuery = new Query("Card").setFilter(validFilter);
		PreparedQuery objPreparedQuery = datastore.prepare(objQuery);
		Entity card = objPreparedQuery.asSingleEntity();
		
		card.setProperty("Active", false);
		datastore.put(card);
		
		try {
			response.getWriter().print("cancel");
		} catch (IOException e) {
			logger.log(Level.WARNING, e.getMessage() + "**" + e.getStackTrace() + "**");
			response.getWriter().print("error");
		}
		
	}

	private void confirm(HttpServletRequest request){
		String writer = request.getParameter("writer");
		String userId = request.getParameter("userId");
		Date today = DateNow.getDateTime();
		
		Filter writerFilter = new FilterPredicate("Writer", FilterOperator.EQUAL, writer);
		Filter userFilter = new FilterPredicate("UserId", FilterOperator.EQUAL, userId);
		Filter activeFilter = new FilterPredicate("Active", FilterOperator.EQUAL, true);
		Filter validFilter = CompositeFilterOperator.and(writerFilter, userFilter, activeFilter);
		
		Query objQuery = new Query("Card").setFilter(validFilter);
		PreparedQuery objPreparedQuery = datastore.prepare(objQuery);
		Entity card = objPreparedQuery.asSingleEntity();
		
		card.setProperty("Active", false);
		card.setProperty("IsWrite", true);
		card.setProperty("TimeWrite", String.format("%tr", today));
		datastore.put(card);
		
		JSONObject objJsonObject = new JSONObject();
		try{
			objJsonObject.put("type", "writed");
		} catch(JSONException e){
			logger.log(Level.WARNING ,e.getMessage());
		}
		
		ChannelServlet.sendMessage(userId, objJsonObject.toString());
	}
	
}
