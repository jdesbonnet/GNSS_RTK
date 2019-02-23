/**
 * Latitude, longitude point on the Earth's surface.
 * 
 * @author Joe Desbonnet
 *
 */
public class GeoPoint {

	public Double latitude;
	public Double longitude;
	
	public GeoPoint (Double latitude, Double longitude) {
		this.latitude = latitude;
		this.longitude = longitude;
	}
	
	public String toString () {
		return "(" + latitude + "," + longitude + ")";
	}

	public String toJSON () {
		return "[" + latitude + "," + longitude + "]";
	}
	
}
