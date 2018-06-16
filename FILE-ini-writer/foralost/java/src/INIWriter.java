import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.HashMap;

/**
 * INI Writer
 * 
 * This Class is used for writing INI File using maps.
 */
public class INIWriter
{
	/**
	 * Function for writing INI File.
	 * 
	 * @param szFileName Name of the new INI File, truncates existing.
	 * @param gHeader The structure of INI File stored in a INIHeader
	 * @return Logic value indicating success or failure.
	 */
	static public boolean writeINI(String szFileName, INIHeader gHeader) throws FileNotFoundException
	{
		if(gHeader.isEmpty())
			return false;
		
		PrintWriter wFILE = new PrintWriter(szFileName);
		
		for(String szSectionName : gHeader.keySet())
		{
			if(!szSectionName.equals("GLOBAL"))
				wFILE.println('[' + szSectionName + ']');
			
			for(String szKeyName : gHeader.get(szSectionName).keySet()) 
			{
				if(!szKeyName.startsWith("COM_"))
					wFILE.println(szKeyName + " = " + gHeader.get(szSectionName).get(szKeyName));
				else 
					wFILE.println(";" + gHeader.get(szSectionName).get(szKeyName));
			}
			wFILE.println();
		}
		
		wFILE.close();
		return true;
	}
	
}


/**
 * This class is designed to imitate the section of an direct content of INI Section.
 * Mainly used to be added with INIHeader.
 * 
 * Wrapped up HashMap which consists of both String for key and value.
 * Where key is accordingly a key of a parameter in INI File and value correspondingly the same.
 */
class INISection extends HashMap<String, String>{
	private static final long serialVersionUID = 1L;
	
}

/**
 * This class is designed to imitate the structure of an INI File.
 * Key is the name of the Section and INISection value as its value/structure.
 * 
 * Wrapped up HashMap which consists of String for key and INISection for value.
 * 
 */
class INIHeader extends HashMap<String, INISection> {
	private static final long serialVersionUID = 1L; 
	
}

