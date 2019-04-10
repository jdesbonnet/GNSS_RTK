import java.io.*;

class GarminBinaryInputStream extends Object {
   static final byte DLE=0x10;
   static final byte ETX=0x03;
   static final byte DAT=0x01;

   static final byte POS_REC=0x28;
   static final byte RCV_REC=0x29;
   static final byte EPH_REC=0x2a;
   
   private byte state=0;
   private byte chksum=0;
   
   InputStream inputStream;
   
   GarminBinaryInputStream (InputStream inputStream) {
      
      this.inputStream=inputStream;
   
   }


   void getRecord () throws IOException {
      System.out.println ("Reading record...");
      byte recordType = get_data_byte();
      byte recordLength = get_data_byte();
      byte[] buf = new byte[recordLength];

      
      for (int i=0; i<recordLength; i++) {
	 buf[i] = get_data_byte();
      }
      
      byte checksum = get_data_byte();
      
      switch (recordType) {
       case RCV_REC:
	 System.out.println ("Got receiver record.");
	 break;
       case POS_REC:
	 System.out.println ("Got position record.");
	 break;
       default:
	 System.out.println ("Got unknown record.");
      }
      
   }
   
   double readDouble () throws IOException {
      long l;
      l =   (long)get_data_byte();
      l += ((long)get_data_byte())<<8;
      l += ((long)get_data_byte())<<16;
      l += ((long)get_data_byte())<<32;
      l += ((long)get_data_byte())<<40;
      l += ((long)get_data_byte())<<48;
      l += ((long)get_data_byte())<<56;
      l += ((long)get_data_byte())<<72;
      return Double.longBitsToDouble(l);
   }
	
   private byte get_data_byte () throws IOException {
      byte c;
      byte[] buf = new byte[1];
      while (true) {
	 inputStream.read(buf);
	 //System.out.println ("byte " + buf[0]);
	 c=buf[0];
	 chksum +=c ;
	 if (state == DAT) {
	    if (c == DLE) {
	       state=DLE;
	    } else {
	       return c;
	    }
	 } else if (state == DLE) {
	    if (c == ETX) {
	       state=ETX;
	    } else {
	       state=DAT;
	       return c;
	    }
	 } else if (state == ETX) {
	    if (c == DLE) {
	       state=DLE;
	    }
	 }
      }
   }
   
}
