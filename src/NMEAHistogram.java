
import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 *
 * 
 * @author Joe Desbonnet
 *
 */
public class NMEAHistogram {

	public static void main(String arg[]) throws Exception {
		
		double from = Double.parseDouble(arg[0]);
		double to = Double.parseDouble(arg[1]);
		double binSize = Double.parseDouble(arg[2]);
		
		int nBins = (int)((to-from)/binSize);
		
		int[] count = new int[nBins];
		

		//System.out.println ("from " + from + " to " + to + " binSize " + binSize + " nBins" + nBins);
		BufferedReader r = new BufferedReader(new InputStreamReader(System.in));

		String line;

		
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

			
			double altitude = Double.parseDouble(p[9]);
			
			if (nSatellite < 7) {
				continue;
			}
			
			int bin = (int)((altitude-from)/binSize);
			if (bin < nBins && bin>=0) {
				count[bin]++;
			}
		}
		
		
		for (int i = 0; i < nBins; i++) {
			System.out.println ("" + (from + binSize*i) + " " + count[i]);
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
