
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Play back NMEA log file in real time. Rather simplistic. Does not handle midnight rollover.
 *
 * TODO: option to change NMEA time/date to current time.
 * 
 * @author Joe Desbonnet
 *
 */
public class NMEAPlayback {

	// GGA sentence timestamp format.
	private static final SimpleDateFormat timeFormat = new SimpleDateFormat("HHmmss");
	
	public static void main (String arg[]) throws Exception {
		
		BufferedReader r = new BufferedReader(new InputStreamReader(System.in));
		Writer w = new BufferedWriter(new OutputStreamWriter(System.out));
		//Writer w = System.out;

		String line;
		String[] p;

		// Find first GGA sentence 
		line = skipToNextGGA(r, null);
		p = line.split(",");
		Date logStartTime = timeFormat.parse(p[1]);
		
		long sysTimeAtStartOfPlayback = System.currentTimeMillis();
		
		// Write the GGA sentence
		w.write(line + "\n");
		
		
		while ((line = skipToNextGGA(r, w)) != null) {
			
			// line now has GGA sentence at start of next sequence. How long 
			// to wait before echoing?
			p = line.split(",");
			Date logRecordTime = timeFormat.parse(p[1]);
			long offsetFromStartOfLog = logRecordTime.getTime() - logStartTime.getTime();
			long sysTimeOfNextRecord = sysTimeAtStartOfPlayback + offsetFromStartOfLog;
			long delay = sysTimeOfNextRecord - System.currentTimeMillis();
			if (delay < 0 ) {
				delay=0;
			}
			Thread.sleep(delay);
			
			// Write GGA sentence
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
	private static final String skipToNextGGA (BufferedReader r, Writer w) throws IOException {
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
	
	/**
	 * Read next valid NMEA0183 sentence. Return null if end of file.
	 * 
	 * @param r
	 * @return
	 * @throws IOException
	 */
	
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
