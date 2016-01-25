
import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * Calculate the mean latitude, longitude, altitude from NMEA log from static
 * receiver.
 * 
 * @author Joe Desbonnet
 *
 */
public class NMEACalcMean {

	public static void main(String arg[]) throws Exception {

		BufferedReader r = new BufferedReader(new InputStreamReader(System.in));

		String line;
		
		double sumLatitude = 0;
		double sumLongitude = 0;
		double sumAltitude = 0;
		int nSamples=0;

		while ((line = r.readLine()) != null) {
			String[] p = line.split(",");
			if (p.length < 2) {
				continue;
			}
			if (!"$GPGGA".equals(p[0])) {
				continue;
			}

			String timestamp = p[1];

			if (p[2].length() == 0) {
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

			int nSatellite = Integer.parseInt(p[7]);

			
			int fixType = Integer.parseInt(p[6]);
			String fixTypeStr = "?";
			switch (fixType) {
			case 0:
				fixTypeStr = "0";
				fixTypeStr = "INVALID";
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

			
			double altitude = Double.parseDouble(p[9]);
			
			if (nSatellite < 6) {
				continue;
			}
			
			sumLatitude += latitude;
			sumLongitude += longitude;
			sumAltitude += altitude;
			
			nSamples++;


			
		}
		
		if (nSamples>0) {
			double N = (double)nSamples;
			System.out.println ( (sumLatitude / N) + " " + (sumLongitude/N) + " " + (sumAltitude/N) );
		}

	}

	/**
	 * Convert latitude/longitude format in NMEA0183 (ddmm.mmm) to decimal
	 * degrees. Eg 5330.00 -> 53.5
	 * 
	 * @param degMin
	 * @return
	 */
	private static double toDegrees(double degMin) {
		int degrees = (int) (degMin / 100);
		double minutes = degMin - degrees * 100;
		return (double) degrees + minutes / 60;
	}
}
