package door_man;

import java.util.HashSet;
import java.util.Set;
import java.util.TreeSet;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * UsersStore para almacenar a los usuarios logueados para la recepcion
 * de las actividades del sistema.
 * @author : Irvin Ponce Pérez
 *
 */
public class UsersStore {
	
	private Set<String> usersList = new HashSet<String>();
	private static UsersStore instance;
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
	 */
	void addUser(String user){
		if (!usersList.contains(user)){
			logger.log(Level.INFO,"User {0} is added to the list",user);
			usersList.add(user);	
		}
	}
	
	/**
	 * Remueve el usuario de la lista.
	 * @param user : el usuario que necesita ser eliminado.
	 */
	void removeUser(String user){
		logger.log(Level.INFO,"User {0} is removed from the list",user);
		usersList.remove(user);
	}
	
	/**
	 * Devuelve la lista completa de usuarios.
	 * @return devuelve el objeto users.
	 */
	Set<String> getUsers(){
		return new TreeSet<String>(usersList);
	}
}
