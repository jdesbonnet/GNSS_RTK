/**
 * Given a uBlox UBX message payload a hex bytes in arg, construct
 * a printf string that can be used in bash etc to send that message
 * with the checksum. Ref uBlox 8/M8 protocol manual section 32.4.
 *
 * Example:
 * java UBX 01 06 08 00 F5 4D 00 00 00 01 00 00
 */


import java.util.List;
import java.util.ArrayList;

public class UBX {

	private static final int BINARY=0;
	private static final int PRINTF=1;

	private static void usage () {
		System.err.println("java UBX -format binary|printf byte0 byte1 byte2...");
		System.err.println("");
		System.err.println("Example:");
		System.err.println("java UBX -format binary 05 01 03");
	}

	public static void main (String[] args) {

		if (args.length == 0) {
			usage();
			return;
		}

		// Checksum: 8-bit Fletcher Algorithm (as used in TCP / RFC 1145).
		int A = 0;
		int B = 0;

		int outputFormat = BINARY;

		int[] bytes = new int[1024];

		int nbytes = 0;
		for (int i = 0; i < args.length; i++) {
			if (args[i].equals("-f")) {
				String format = args[++i];
				if ("binary".equals(format)) {
					outputFormat = BINARY;
				} else if ("printf".equals(format)) {
					outputFormat = PRINTF;
				}
			} else {
				bytes[nbytes++] = Integer.parseInt(args[i],16);
			}
		}


		// Write frame header
		switch (outputFormat) {
			case BINARY:
				System.out.write(0xb5);
				System.out.write(0x62);
				System.out.flush();
				break;
			case PRINTF:	
				System.out.print("printf $'\\xb5\\x62");
				break;
		}


		for (int i = 0; i < nbytes; i++) {

			A += bytes[i];
			B += A;

			switch (outputFormat) {
			case BINARY:
				System.out.write(bytes[i]);
				System.out.flush();
				break;
			case PRINTF:
				System.out.print (String.format("\\x%02x",bytes[i]));
				break;
			}
		}

		switch (outputFormat) {
			case BINARY:
				System.out.write (A & 0xff);
				System.out.write (B & 0xff);
				System.out.flush();
				break;
			case PRINTF:
				System.out.println (String.format("\\x%02x\\x%02x'",A&0xff,B&0xff));
				break;
		}


		System.out.flush();
	}
}

