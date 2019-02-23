
import java.io.BufferedReader;
import java.io.InputStreamReader;


public class NMEAToLatLngAlt {

	public static void main (String arg[]) throws Exception {
		
		//GeoPoint p1 = new GeoPoint(53.282665, -8.982498);
		GeoPoint p1 = new GeoPoint(53.280435, -9.057866);

		final double lat0 = 53.28263728178473;
		final double lng0 = -8.982488730833833;

		double sumLatitude = 0;
		double sumLongitude = 0;
		double sumDLat2 = 0;
		double sumDLng2 = 0;
		
		int N=0;
		
		BufferedReader r = new BufferedReader(new InputStreamReader(System.in));
		
		String line;
		
		while (  (line = r.readLine()) != null) { 
		String[] p = line.split(",");
		if (p.length < 2) {
			continue;
		}
		if (!"$GPGGA".equals(p[0])) {
			continue;
		}
		
		String timestamp = p[1];
		
		if (p[2].length()==0) {
			continue;
		}
		
		double latitude = toDegrees(Double.parseDouble(p[2]));
		if ("S".equals(p[3])) {
			latitude *= -1;
		}
		double longitude = toDegrees(Double.parseDouble(p[4]));
		if ("W".equals(p[5])) {
			longitude *= -1;
		}
				
		double dx = GeoUtil.distanceFrom(p1, new GeoPoint(p1.latitude, longitude));
		if (longitude < p1.longitude) {
			dx = -dx;
		}
		double dy = GeoUtil.distanceFrom(p1, new GeoPoint(latitude, p1.longitude));
		if (latitude < p1.latitude) {
			dy = -dy;
		}
		
		int fixType = Integer.parseInt(p[6]);
		String fixTypeStr="?";
		switch (fixType) {
		case 0:
			fixTypeStr = "0";
				fixTypeStr="INVALID";
			break;
		case 1:
			fixTypeStr = "NGPS";
			break;
		case 2:
			fixTypeStr = "DGPS";
			break;
		case 5:
			fixTypeStr = "FloatRTK";
			break;
		}
				
		int nSatellite = Integer.parseInt(p[7]);
		
		if (nSatellite >= 8) {
			sumLatitude += latitude;
			sumLongitude += longitude;
			sumDLat2 += latitude - lat0;
			sumDLng2 += longitude - lng0;
			N++;
		}
		
		System.out.println(timestamp + " " + latitude + " " + longitude + " " + dx + " " + dy 
				+ " " + fixTypeStr + " " + nSatellite);
		}
		
		double meanLatitude = sumLatitude / N;
		double meanLongitude = sumLongitude / N;
		System.err.println("Mean latitude: " + meanLatitude);
		System.err.println("Mean longitude: " + meanLongitude);
		System.err.println("Sigma latitude: " + Math.sqrt(sumDLat2/(N-1)));
		System.err.println("Sigma longitude: " + Math.sqrt(sumDLng2/(N-1)));
		
	}
	
	/**
	 * Convert latitude/longitude format in NMEA0183 (ddmm.mmm) to decimal degrees.
	 * Eg 5330.00 -> 53.5
	 * @param degMin
	 * @return
	 */
	private static double toDegrees (double degMin) {
		int degrees = (int)(degMin/100);
		double minutes = degMin - degrees*100;
		return (double)degrees + minutes/60;
	}
}
