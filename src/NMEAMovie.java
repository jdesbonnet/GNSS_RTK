
import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;


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
public class NMEAMovie {

	public static void main(String arg[]) throws Exception {
		
		Options options = new Options();
		options.addOption("help", false, "display this message");
		options.addOption("numsv", true, "filter for fixes using this or greater number of satellites");
		options.addOption("dgpsonly", true, "filter for fixes using DGPS only");
		options.addOption("latbinsize", true, "bin size in degrees");
		options.addOption("lngbinsize", true, "bin size in degrees");
		options.addOption("framelength", true, "single frame duration in seconds");
		options.addOption("lng0", true, "bottom left corner longitude");
		options.addOption("lat0", true, "bottom left corner latitude");
		options.addOption("lng1", true, "top right corner longitude");
		options.addOption("lat1", true, "top right corner latitude");

		CommandLineParser parser = new DefaultParser();
		CommandLine cmd = parser.parse( options, arg);
		
		if (cmd.hasOption("help")) {
			HelpFormatter formatter = new HelpFormatter();
			formatter.printHelp( "NMEAHistogram", options );
			return;
		}
		
		Double latBinSize = new Double(cmd.getOptionValue("latbinsize","0.0001"));
		Double lngBinSize = new Double(cmd.getOptionValue("lngbinsize","0.0001"));		

		Integer frameLength = new Integer(cmd.getOptionValue("framelength","60"));		

		Double lat0 = new Double(cmd.getOptionValue("lat0"));
		Double lng0 = new Double(cmd.getOptionValue("lng0"));
		Double lat1 = new Double(cmd.getOptionValue("lat1"));
		Double lng1 = new Double(cmd.getOptionValue("lng1"));

		Integer numberOfSv = null;
		if (cmd.hasOption("numsv")) {
			numberOfSv = new Integer(cmd.getOptionValue("numsv"));
		}


		int nLatBin = (int)((lat1-lat0)/latBinSize);
		int nLngBin = (int)((lng1-lng0)/lngBinSize);
		

		int latBin0 = (int)(lat0 / latBinSize);
		int lngBin0 = (int)(lng0 / lngBinSize);
				
		//System.out.println ("from " + from + " to " + to + " binSize " + binSize + " nBins" + nBins);
		BufferedReader r = new BufferedReader(new InputStreamReader(System.in));

		String line;

		Counter[][] counters = new Counter[nLatBin][nLngBin];

		
		int totalCount = 0;
		
		int frameNumber = 1;
		
		int prevTimeBin = 0;
		
		String date="";
		
		SimpleDateFormat timestampFormat = new SimpleDateFormat("ddMMyy-HHmmss");
	 	
		ArrayList<double[]> latLngPoints = new ArrayList<>();
		
		while ((line = r.readLine()) != null) {
			String[] p = line.split(",");
			if (p.length < 2) {
				continue;
			}
			
			if ("$GPRMC".equals(p[0])) {
				date = p[9];
				//System.err.println ("date=" + date);
				continue;
			}
			if (!"$GPGGA".equals(p[0])) {
				continue;
			}
			
			if (p.length < 9) {
				continue;
			}
			
			if (p[2].length() == 0) {
				continue;
			}
			
			Date ts = timestampFormat.parse("" + date + "-" + p[1]);
			//System.err.println("timestamp=" + ts);
			
			int timeBin = (int)(ts.getTime()/(1000L*frameLength));
			
			if (timeBin > prevTimeBin) {
				// Write frame
				System.err.println("Writing frame " + frameNumber);
				String zeroPaddedFrameNumber = String.format("%06d", frameNumber);
				File frameHeatMap = new File("f" + zeroPaddedFrameNumber + ".hm.dat");
				FileWriter w = new FileWriter(frameHeatMap);
				int i,j;
				for (j = 0; j < nLatBin; j++) {
					for (i = 0; i < nLngBin; i++) {
						Counter counter = counters[j][i];
						w.write("" + (j+latBin0)*latBinSize 
								+ " " 
								+ (i+lngBin0)*lngBinSize 
								+ " " 
								+ (counter==null ? "-1" : counter.count)
								+ "\n");
					}
				}
				w.close();
				
				File framePoints = new File("f" + zeroPaddedFrameNumber + ".pt.dat");
				w = new FileWriter(framePoints);
				
				for (double[] point : latLngPoints) {
					w.write(point[0] + " " + point[1] + "\n");
				}
				w.close();
				latLngPoints.clear();
				
				File gnuplotFile = new File ("f" + zeroPaddedFrameNumber + ".gp");
				w = new FileWriter(gnuplotFile);
				w.write("load 'movie.gp'\n");
				w.write("set title 'Frame " + zeroPaddedFrameNumber + "'\n");
				w.write("plot '" + frameHeatMap.getName() + "' with image, \\n" );
				w.write("'" + framePoints + "' with points");
				w.close();
				
				frameNumber++;
				prevTimeBin = timeBin;
				
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
			
			if (latitude < lat0 || latitude > lat1 || longitude < lng0 || longitude > lng1) {
				continue;
			}
			
			
			Integer latBin = (int)(latitude / latBinSize);
			Integer lngBin = (int)(longitude / lngBinSize);
			
			Counter counter = counters[latBin-latBin0][lngBin-lngBin0];
			if (counter == null) {
				counter = new Counter();
				counters[latBin-latBin0][lngBin-lngBin0] = counter;
				System.err.println("creating counter for " + (latBin-latBin0) + " " + (lngBin-lngBin0));
			}
			counter.count++;
			
			double[] point = {latitude, longitude};
			latLngPoints.add(point);
			
			totalCount++;
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
	
	private static class Counter {
		public int count = 0;
	}
}
