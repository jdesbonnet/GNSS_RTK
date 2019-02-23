
import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


/**
 * Parse NMEA0183 file for $GPGSV and generate azimuth, elevation, SNR heat map.
 * 
 * @author Joe Desbonnet
 *
 */
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
		
		Set<Integer> svNumbers = new HashSet<>();
		
		// Use this to keep track of azimuth,altitude where the SV is at highest in sky
		Map<Integer,AltAzi> peakAlt = new HashMap<>();

		
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
					Integer svn = new Integer(p[i * 4 + 4]);
					svNumbers.add(svn);
					

					
					
					int altitudeInt = Integer.parseInt(p[i * 4 + 5]);
					int azimuthInt = Integer.parseInt(p[i * 4 + 6]);
					int snr = Integer.parseInt( p[i * 4 + 7]);
					
					AltAzi altazi = peakAlt.get(svn);
					if (altazi == null) {
						altazi = new AltAzi();
						peakAlt.put(svn, altazi);
					}
					if (altitudeInt > altazi.altitude) {
						altazi.altitude = altitudeInt;
						altazi.azimuth = azimuthInt;
					}
					
					
					
					if (heatmap != null && altitudeInt >= 0) {
						heatmap[altitudeInt][azimuthInt] += snr;
						heatmapc[altitudeInt][azimuthInt]++;
					}
					
					double elevation = (double)altitudeInt;
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
		
		// Display SV numbers encountered
		for (Integer svNumber : peakAlt.keySet()) {
			System.err.print("SV" + svNumber);
			System.err.println(""
					+ " " + peakAlt.get(svNumber).altitude 
					+ " " + peakAlt.get(svNumber).azimuth
					);
		}
		
	}
	
	private static class AltAzi {
		/**
		 * Altitude (angle from horizon) of SV in degrees
		 */
		int altitude=0; // aka 'elevation'
		
		/**
		 * Azimuth of SV in degrees
		 */
		int azimuth=0;
	}
}
