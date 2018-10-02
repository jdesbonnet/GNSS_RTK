/**
 * Given a uBlox UBX message payload a hex bytes in arg, construct
 * a printf string that can be used in bash etc to send that message
 * with the checksum. Ref uBlox 8/M8 protocol manual section 32.4.
 *
 * Example:
 * java UBX 01 06 08 00 F5 4D 00 00 00 01 00 00
 */
public class UBX {

	public static void main (String[] arg) {

		// Checksum: 8-bit Fletcher Algorithm (as used in TCP / RFC 1145).
		int A = 0;
		int B = 0;

		System.out.print("printf $'\\xb5\\x62");

		for (String hex : arg) {
			int hexv = Integer.parseInt(hex,16);
			A += hexv;
			B += A;
			System.out.print (String.format("\\x%02x",hexv));
		}
		System.out.println (String.format("\\x%02x\\x%02x'",A&0xff,B&0xff));
	}
}

