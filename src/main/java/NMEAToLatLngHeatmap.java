import java.io.BufferedReader;
import java.io.InputStreamReader;

public class NMEAToLatLngHeatmap {

	public static void main(String arg[]) throws Exception {

		final double lat0 = 53.2824;
		final double lng0 = -8.9830;
		final double lat1 = 53.2830;
		final double lng1 = -8.9820;
		final double latBinSize = 0.000005;
		final double lngBinSize = 0.000005;
		
		// NUIG
		/*
		final double lat0 = 53.2800;
		final double lng0 = -9.0580;
		final double lat1 = 53.2830;
		final double lng1 = -9.0560;
		final double latBinSize = 0.0000025;
		final double lngBinSize = 0.0000025;
		*/
		
		final int nLatBin = (int) ((lat1 - lat0) / latBinSize);
		final int nLngBin = (int) ((lng1 - lng0) / lngBinSize);
		int[][] count = new int[nLatBin][nLngBin];

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


			int y = (int) ((latitude - lat0) / latBinSize);
			int x = (int) ((longitude - lng0) / lngBinSize);
			
			if (x < 0 || x >= nLngBin) continue;
			if (y < 0 || y >= nLatBin) continue;

			count[y][x]++;

		}

		int i,j;
		for (j = 0; j < nLatBin; j++) {
			for (i = 0; i < nLngBin; i++) {
				System.out.println( (j*latBinSize+lat0)
						+ " "
						+ (i*lngBinSize+lng0)
						+ " "
						+ count[j][i]
						);
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
