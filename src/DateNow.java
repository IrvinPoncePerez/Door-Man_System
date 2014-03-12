package door_man;

import java.util.Date;
import java.util.TimeZone;

public class DateNow {
	
	public static Date getDateTime(){
		
		TimeZone objTimeZone = TimeZone.getTimeZone("America/Mexico_City");
		
		long currentTime = new Date().getTime();
		long convertedTime = currentTime + objTimeZone.getOffset(currentTime);
				
		return new Date(convertedTime);
		
	}
	
	public static long getLongTime(){
		TimeZone objTimeZone = TimeZone.getTimeZone("America/Mexico_City");
		
		long currentTime = new Date().getTime();
		long convertedTime = currentTime + objTimeZone.getOffset(currentTime);
				
		return convertedTime;
	}

}
