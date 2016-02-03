
import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStreamReader;


public class NMEAToSV {

	public static void main (String arg[]) throws Exception {
		

		File heatMapFile = null;
		int[][] heatmap = null;
		int[][] heatmapc = null;
		
		for (int i = 0; i < arg.length; i++) {
			if ("-heatmap".equals(arg[i])) {
				heatMapFile = new File(arg[++i]);
				heatmap = new int[90][360];
				heatmapc = new int[90][360];
			}
		}

		BufferedReader r = new BufferedReader(new InputStreamReader(System.in));
		
		String line;
		String timestamp=null;
		
		double pointFudgeFactor = 1;
		
		while (  (line = r.readLine()) != null) { 
			
			// Remove checksum
			int checksumIndex = line.lastIndexOf('*');
			
			// Incomplete line
			if (checksumIndex < 0) {
				continue;
			}
			
			if (line.startsWith("$GPGGA")) {
				String[] p = line.split(",");
				timestamp = p[1];
				continue;
			}
			if (! line.startsWith("$GPGSV")) {
				continue;
			}
			

			
			line = line.substring(0,checksumIndex);

			String[] p = line.split(",");

			
			int nrec = (p.length-4)/4;
			for (int i = 0; i < nrec; i++) {
				// timestamp prn elevation azimuth snr
				try {
					int elevationInt = Integer.parseInt(p[i * 4 + 5]);
					int azimuthInt = Integer.parseInt(p[i * 4 + 6]);
					int snr = Integer.parseInt( p[i * 4 + 7]);
					
					if (heatmap != null && elevationInt >= 0) {
						heatmap[elevationInt][azimuthInt] += snr;
						heatmapc[elevationInt][azimuthInt]++;
					}
					
					double elevation = (double)elevationInt;
					double azimuth = (double)azimuthInt;
					elevation += Math.random() * pointFudgeFactor - pointFudgeFactor / 2;
					azimuth += Math.random() * pointFudgeFactor - pointFudgeFactor / 2;
					System.out.println(
							timestamp + " " + p[i * 4 + 4] + " " + elevation + " " + azimuth + " " + p[i * 4 + 7]);
				} catch (NumberFormatException e) {
					// ignore line
				} catch (ArrayIndexOutOfBoundsException e) {
					e.printStackTrace();
					System.err.println("line=" + line);
				}
			}
					
		}
		
		if (heatMapFile != null) {
			FileWriter w = new FileWriter(heatMapFile);
			FileWriter w2 = new FileWriter("count.dat");
			int i,j;
			for (j = 0; j < 90; j++) {
				for (i = 0; i < 360; i++) {
					if (heatmapc[j][i]>500) {
						//System.err.println("large count: " + i + " " + j + " " + heatmapc[j][i] + "\n");
						w2.write( i + " " + j + " 0\n");
					} else {
						w2.write( i + " " + j + " " + heatmapc[j][i] + "\n");
					}
					if (heatmapc[j][i]==0) {
						w.write(i + " " + j + " 0\n");
					} else {
						w.write(i + " " + j + " " + ((double)heatmap[j][i]/(double)heatmapc[j][i]) + "\n");
					}
				}
			}
			w.close();
			w2.close();
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
