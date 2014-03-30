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

/**
 * Este Servlet crea el canal con el Servidor AppEngine y Obtiene el token.
 * 
 * @author Irvin Ponce Perez
 */
@SuppressWarnings("serial")
public class ChannelServlet extends HttpServlet{
	
	private static final Logger logger = Logger.getLogger(ChannelServlet.class.getCanonicalName());
	private static final ChannelService channelService = ChannelServiceFactory.getChannelService();
	
	/**
	 * obtiene el token para conectar y escuchar el canal.
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String userId = request.getParameter("userId");
		String name = request.getParameter("name");
		
		UsersStore objUsersStore = UsersStore.getInstance();
		if (userId != null && !"".equals(userId)) {
			String token = createChannel(userId);
			objUsersStore.addUser(userId, name);
			writeIntoChannel(response, token);
		}
	}
	
	/**
	 * Crea el token del canal para el usuario 
	 * @param userId el usuario para quien el token es creado.
	 * @return The token string is returned
	 */
	public String createChannel(String userId){
		try{
			logger.log(Level.INFO, "Creating a channel for {0}", userId);
			return channelService.createChannel(userId);
		} catch (ChannelFailureException channelFailureException) {
			logger.log(Level.WARNING, "Error creating the channel.");
			return channelService.createChannel(userId);
		} catch (Exception otherException){
			logger.log(Level.WARNING, "Unknown exception while creating channel");
			return channelService.createChannel(userId);
		}
	}
	
	/**
	 * Escribe el token en el texto de respuesta.
	 * 
	 * @param response el objeto response a para escribir el texto de respuesta.
	 * @param token el cual necesita ser escrito en la respuesta.
	 * @throws IOException
	 */
	public void writeIntoChannel(HttpServletResponse response, String token){
		try{
			logger.log(Level.INFO, "writing the token {0} to the output", token);
			response.getWriter().print(token);
		} catch (IOException ioException){
			logger.log(Level.WARNING, "Exception while writing output ");
		} catch(Exception exception){
		  logger.log(Level.WARNING, "Unknow exception while writing output ");
		}
	}

	/**
	 * Envia el mensaje al canal asignado al usuario dado.
	 * @param user
	 * @param message
	 */
	public static void sendMessage(String user, String message){
		try{
			channelService.sendMessage(new ChannelMessage(user, message));			
		}
		catch (ChannelFailureException ex) {
			logger.log(Level.WARNING, "Failed in sending message to channel");
		}
	}
	
}
