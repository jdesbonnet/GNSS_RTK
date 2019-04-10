import java.io.*;

public class TestGarmin {
   
   public static void main (String args[]) {
      FileInputStream inputStream;
      GarminBinaryInputStream garbin;
      System.out.println ("Opening file " + args[0]);
      try {
	 inputStream = new FileInputStream (args[0]);
	 garbin = new GarminBinaryInputStream (inputStream);
      } catch (Exception e) {
	 System.out.println ("Error opening input stream: " + e);
	 return;
      }

      while (true) {
	 try {
	    garbin.getRecord();
	 } catch (Exception e) {
	    System.out.println ("Error: " + e);
	    break;
	 }
      }
      
   }
}
