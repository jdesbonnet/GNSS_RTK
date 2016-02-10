
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;

/**
 *
 * 
 * @author Joe Desbonnet
 *
 */
public class NMEAHistogram {

	public static void main(String arg[]) throws Exception {
		
		Options options = new Options();
		options.addOption("help", false, "display this message");
		options.addOption("dimension", true, "which dimension to use: 'latitude' | 'longitude' | 'altitude'");
		options.addOption("numsv", true, "filter for fixes using this or greater number of satellites");
		options.addOption("dgpsonly", true, "filter for fixes using DGPS only");
		options.addOption("binsize", true, "bin size (meters for altitude, degrees for latitude, longitude)");
		options.addOption("range", true, "'auto' or from,to");

		CommandLineParser parser = new DefaultParser();
		CommandLine cmd = parser.parse( options, arg);
		
		if (cmd.hasOption("help")) {
			HelpFormatter formatter = new HelpFormatter();
			formatter.printHelp( "NMEAHistogram", options );
			return;
		}
		
		String dimension = cmd.getOptionValue("dimension","altitude");
		Double binSize = new Double(cmd.getOptionValue("binsize","1.0"));		
		String range = cmd.getOptionValue("range","auto");
		
		Double from = null;
		Double to = null;
		if (!"auto".equals(range)) {
			String[] p = range.split(",");
			from = new Double(p[0]);
			to = new Double(p[1]);
		}
		
		Integer numberOfSv = null;
		if (cmd.hasOption("numsv")) {
			numberOfSv = new Integer(cmd.getOptionValue("numsv"));
		}

	
		//int nBins = (int)((to-from)/binSize);
		
		//int[] count = new int[nBins];
		

		//System.out.println ("from " + from + " to " + to + " binSize " + binSize + " nBins" + nBins);
		BufferedReader r = new BufferedReader(new InputStreamReader(System.in));

		String line;

		Map<Integer,Counter> counters = new HashMap<>();
		int highestBin = Integer.MIN_VALUE;
		int lowestBin = Integer.MAX_VALUE;
		
		int totalCount = 0;
		
		while ((line = r.readLine()) != null) {
			String[] p = line.split(",");
			if (p.length < 2) {
				continue;
			}
			if (!"$GPGGA".equals(p[0])) {
				continue;
			}
			
			if (p.length < 9) {
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
			
			if (numberOfSv!= null && nSatellite < numberOfSv) {
				continue;
			}
			
			double dim=0;
			switch (dimension) {
			case "latitude":
				dim = latitude;
				break;
			case "longitude":
				dim = longitude;
				break;
			case "altitude":
				dim = altitude;
				break;
			}
			Integer bin = (int)(dim / binSize);
			
			if (bin > highestBin) highestBin = bin;
			if (bin < lowestBin) lowestBin = bin;

			if (!counters.containsKey(bin)) {
				counters.put(bin, new Counter());
			}
			counters.get(bin).count++;
			
			totalCount++;
		}
		
	
		System.err.println("lowestBin=" + lowestBin + " hightestBin=" + highestBin);
		
		// Find bounds where 99% of points are within
		int a=lowestBin,b=highestBin;
		int s=0;
		for (int i = lowestBin; i < highestBin; i++) {
			s +=  (counters.containsKey(i) ? counters.get(i).count : 0);
			if (s >= totalCount/200) {
				a = i;
				break;
			}
		}
		
		s = 0;
		for (int i = highestBin; i >= lowestBin; i--) {
			s +=  (counters.containsKey(i) ? counters.get(i).count : 0);
			if (s >= totalCount/100) {
				b = i;
				break;
			}
		}
		
		System.err.println ("a=" + a + " b=" + b);
		
		for (int i = a; i < b; i++) {
			System.out.println ("" + (i*binSize) 
					+ " " 
					+ (counters.containsKey(i) ? counters.get(i).count : 0)
					);
		}
		
		System.err.println("" + totalCount + " in total");
		

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
	
	private static class Counter {
		public int count = 0;
	}
}
