package door_man;


import java.util.logging.Level;
import java.util.logging.Logger;

import com.google.appengine.api.datastore.DatastoreService;
import com.google.appengine.api.datastore.DatastoreServiceFactory;
import com.google.appengine.api.datastore.Entity;
import com.google.appengine.api.datastore.PreparedQuery;
import com.google.appengine.api.datastore.Query;
import com.google.appengine.api.datastore.Query.CompositeFilterOperator;
import com.google.appengine.api.datastore.Query.Filter;
import com.google.appengine.api.datastore.Query.FilterOperator;
import com.google.appengine.api.datastore.Query.FilterPredicate;

/**
 * UsersStore para almacenar a los usuarios logueados para la recepcion
 * de las actividades del sistema.
 * @author : Irvin Ponce Pérez
 * @param <Entity>
 *
 */
public class UsersStore {
	
	private static UsersStore instance;
	private static DatastoreService datastore = DatastoreServiceFactory.getDatastoreService();
	private static final Logger logger = Logger.getLogger(UsersStore.class.getCanonicalName());
	
	/**
	 * Constructor privado - Singleton - 
	 */
	private UsersStore() {
		
	}
	
	/**
	 * devuelve el objeto singleton de la clase UsersStore.
	 * @return UsersStore object
	 */
	public static UsersStore getInstance() {
		if (instance == null){
			instance = new UsersStore();
		}
		return instance;
	}
	
	/**
	 * Agrega un nuevo usuario
	 * @param user : el usuario que sera agregado a la lista.
	 * @param name : nombre del usuario agregado.
	 */
	void addUser(String userId, String name){
		if (!ifExists(userId, name)){
			add(userId, name);
		} else {
			update(userId);
		}
	}
	
	/**
	 * Remueve el usuario de la lista.
	 * @param user : el usuario que necesita ser eliminado.
	 */
	void removeUser(String user){

	}
	
	/**
	 * Devuelve la lista completa de usuarios activos.
	 * @return devuelve el objeto users.
	 */
	Iterable<Entity> getUsers(){
		Filter activeFilter = new FilterPredicate("Active", FilterOperator.EQUAL, true);
		
		Query objQuery = new Query("User").setFilter(activeFilter);
		PreparedQuery objPreparedQuery = datastore.prepare(objQuery);
		return objPreparedQuery.asIterable();
		
	}
	
	/**
	 * Consulta si existe en usuario agregado.
	 * @param userId : Id del Usuario agregado
	 * @param name : nombre del usuario
	 * @param email : email para trabajo a futuro
	 * @return : true si el usuario existe en la base de datos, false si no existe registro.
	 */
	private boolean ifExists(String userId, String name){
		Filter userFilter = new FilterPredicate("UserId", FilterOperator.EQUAL, userId);
		Filter nameFilter = new FilterPredicate("Name", FilterOperator.EQUAL, name);
		Filter objFilter = CompositeFilterOperator.and(userFilter, nameFilter);
		
		Query objQuery = new Query("User").setFilter(objFilter);
		PreparedQuery objPreparedQuery = datastore.prepare(objQuery);
		Entity user = objPreparedQuery.asSingleEntity();
		if (user != null){
			return true;
		} else {
			return false;
		}
	}
	
	/**
	 * Agrega el usuario al datastore.
	 * @param userId
	 * @param name
	 * @param email
	 */
	private void add(String userId, String name){
		Entity user = new Entity("User");
		
		user.setProperty("UserId", userId);
		user.setProperty("Name", name);
		user.setProperty("Active", true);
		user.setProperty("Enable", true);
		user.setProperty("LastLogin", DateNow.getDateTime());
		
		datastore.put(user);
		logger.log(Level.INFO, "User Id=" + userId + " name=" + name + " added!");
	}
	
	/**
	 * Actualiza la propiedad Active y registra la fecha de logueo.
	 * @param userId
	 */
	private void update(String userId){
		Filter userFilter = new FilterPredicate("UserId", FilterOperator.EQUAL, userId);
		
		Query objQuery = new Query("User").setFilter(userFilter);
		PreparedQuery objPreparedQuery = datastore.prepare(objQuery);
		Entity user = objPreparedQuery.asSingleEntity();
		
		user.setProperty("Active", true);
		user.setProperty("LastLogin", DateNow.getDateTime());
		
		datastore.put(user);
		logger.log(Level.INFO, "User Id=" + userId + " Updated!");
	}
}
