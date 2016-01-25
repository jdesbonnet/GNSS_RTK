

public class DegreesLatLngToMeters {

	public static void main (String[] arg) {
		double latitude = Double.parseDouble(arg[0]);
		double longitude = Double.parseDouble(arg[1]);
		
		GeoPoint p1 = new GeoPoint(latitude, longitude);
		double dx = GeoUtil.distanceFrom(p1, new GeoPoint(latitude,longitude+0.0001));
		double dy = GeoUtil.distanceFrom(p1, new GeoPoint(latitude+0.0001,longitude));
		
		System.out.println ("At latitude,longitude " + latitude + "," + longitude + ":");
		System.out.println ("1 degree latitude = " + Math.round(dy*10000) + "m");
		System.out.println ("1 degree longitude = " + Math.round(dx*10000) + "m");
		System.out.println ("1m north/south= " + (0.0001/dy) + " degrees latitude");
		System.out.println ("1m east/west= " + (0.0001/dx) + " degrees longitude");

	}
}
