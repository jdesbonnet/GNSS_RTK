
import java.io.BufferedReader;
import java.io.InputStreamReader;


public class NMEAToSV {

	public static void main (String arg[]) throws Exception {
		

		BufferedReader r = new BufferedReader(new InputStreamReader(System.in));
		
		String line;
		String timestamp=null;
		
		while (  (line = r.readLine()) != null) { 
			
			
			if (line.startsWith("$GPGGA")) {
				String[] p = line.split(",");
				timestamp = p[1];
				continue;
			}
			if (! line.startsWith("$GPGSV")) {
				continue;
			}
			
			// Remove checksum
			line = line.substring(0,line.lastIndexOf('*'));

			String[] p = line.split(",");

			
			int nrec = (p.length-4)/4;
			for (int i = 0; i < nrec; i++) {
				System.out.println(timestamp + " " + p[i*4+4] + " " + p[i*4+5] + " " + p[i*4+6] + " " + p[i*4+7]);
			}
					
		}
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
