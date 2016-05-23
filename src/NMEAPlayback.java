
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Play back NMEA.
 * @author joe
 *
 */
public class NMEAPlayback {

	private static final SimpleDateFormat timeFormat = new SimpleDateFormat("HHmmss");
	
	public static void main (String arg[]) throws Exception {
		
		BufferedReader r = new BufferedReader(new InputStreamReader(System.in));
		Writer w = new BufferedWriter(new OutputStreamWriter(System.out));
		//Writer w = System.out;

		String line;
		String[] p;

		
		line = skipToNextGGA(r, null);
		p = line.split(",");
		Date logStartTime = timeFormat.parse(p[1]);
		
		long sysTimeAtStartOfPlayback = System.currentTimeMillis();
		w.write(line);
		
		
		while ((line = skipToNextGGA(r, w)) != null) {
			p = line.split(",");
			Date logRecordTime = timeFormat.parse(p[1]);
			long offsetFromStartOfLog = logRecordTime.getTime() - logStartTime.getTime();
			long sysTimeOfNextRecord = sysTimeAtStartOfPlayback + offsetFromStartOfLog;
			long delayNeeded = sysTimeOfNextRecord - System.currentTimeMillis();
			//System.err.println ("delay=" + delayNeeded);
			if (delayNeeded < 0 ) {
				delayNeeded=0;
			}
			Thread.sleep(delayNeeded);
			w.write(line + "\n");
		}
		
	}
	
	/**
	 * Skip to next GGA record in NMEA0183 log file. Optionally echo skipped sentences.
	 * 
	 * @param r Read NMEA0183 sentences from here.
	 * @param w Echo skipped sentences here. Do not echo anything if null.
	 * @return
	 * @throws IOException
	 */
	private static String skipToNextGGA (BufferedReader r, Writer w) throws IOException {
		String line;
		while (  (line = readNextNMEASentence(r)) != null) {
			if (line.startsWith("$GPGGA")) {
				return line;
			}
			if (w != null) {
				w.write(line);
				w.write("\n");
				w.flush();
			}
		}
		return null;
	}
	
	private static final String readNextNMEASentence (BufferedReader r) throws IOException {
		
		String line;
		while (  (line = r.readLine()) != null) { 
						
			// Remove checksum
			int checksumIndex = line.lastIndexOf('*');
			
			// Incomplete line
			if (checksumIndex < 0) {
				continue;
			}
			
			return line;
		}
		
		return null;
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
