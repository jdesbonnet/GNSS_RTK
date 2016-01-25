
import java.util.List;

public class GeoUtil {
	
	public static String pointListToSting(List<GeoPoint> points) {
		
		StringBuffer buf = new StringBuffer();
		
		boolean first = true;
		buf.append("[");
		for (GeoPoint p : points) {
			if (first) {
				first = false;
			} else {
				buf.append(",");
			}
			buf.append ("[" + p.latitude + "," + p.longitude + "]");
		}
		buf.append("]");
		return buf.toString();
		
	}
	
	/**
	 * Calculate distance in meters from one lat,lng point to another.
	 * 
	 * @param p1
	 * @param p2
	 * @return
	 */
	public static double distanceFrom(GeoPoint p1, GeoPoint p2) {

		double lat1 = p1.latitude;
		double lat2 = p2.latitude;
		double lng1 = p1.longitude;
		double lng2 = p2.longitude;

		double earthRadius = 6371000; // meters
		double dLat = Math.toRadians(lat2 - lat1);
		double dLng = Math.toRadians(lng2 - lng1);
		double a = Math.sin(dLat / 2) * Math.sin(dLat / 2) + Math.cos(Math.toRadians(lat1))
				* Math.cos(Math.toRadians(lat2)) * Math.sin(dLng / 2) * Math.sin(dLng / 2);
		double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
		double dist = (earthRadius * c);

		return dist;
	}
	
}
