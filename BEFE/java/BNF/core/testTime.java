package bferris.core;

import java.io.PrintStream;

public class testTime {

  //----------------------------------------------------------------------------
  //
  // Method: test - Test Time class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}
	
  public static void test(String indent) {

    String      ind;
    PrintStream out;
    Time        t1;
    Time        t2;
    Time        et;

    ind = indent + "  ";
    out = System.out;

    out.println(indent+"***Testing Time...");

    t1 = new Time();
    out.println("Started at:  "+t1.GetAs(Time.Hours)+"h"
    		                   +t1.GetAs(Time.Minutes)+"m"
    		                   +t1.GetAs(Time.Seconds)+"s"
    		                   +t1.GetAs(Time.Milliseconds)+"ms");

    out.println(ind+Time.Units.length+" total units");
    t2 = new Time();

    et = t1.Delta(t2);
    out.println(ind+"Elapsed Minutes      = "+et.GetAs(Time.Minutes));
    out.println(ind+"Elapsed Seconds      = "+et.GetAs(Time.Seconds));
    out.println(ind+"Elapsed Milliseconds = "+et.GetAs(Time.Milliseconds));
    out.println(ind+"Day of year          = "+t1.GetDayOfYear());
    out.println(ind+"Day of week          = "+Time.enLongDaysOfWeek[t1.GetDayOfWeek()]);
    out.println("");
    out.println(ind+"Day of month         = "+t1.GetDayOfMonth());
    out.println(ind+"Month of year        = "+Time.enLongMonthsOfYear[t1.GetMonthOfYear()]);
    out.println(ind+"Year                 = "+t1.GetYear());
    out.println("");
    out.println(ind+"Hour                 = "+String.format("%02d",t1.GetHour()));
    out.println(ind+"Minute               = "+String.format("%02d",t1.GetMinute()));
    out.println(ind+"Second               = "+String.format("%02d",t1.GetSecond()));
    if (t1.IsLeapYear())
      out.println(ind+t1.GetYear()+" IS a leap year!");
    else
      out.println(ind+t1.GetYear()+" IS NOT a leap year!");

    t2 = new Time();
    out.println("Finished at: "+t2.GetAs(Time.Hours)+"h"
                               +t2.GetAs(Time.Minutes)+"m"
                               +t2.GetAs(Time.Seconds)+"s"
                               +t2.GetAs(Time.Milliseconds)+"ms");

  }


}
