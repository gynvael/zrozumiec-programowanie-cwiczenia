import java.io.FileNotFoundException;


/* 
 *  This program constructs a INI file using the same method as from C++ (a map with paired Strings).
 */


public class Main { 
	
	public static void main(String[] args)
	{
		INIHeader mainHeader = new INIHeader();
		
		INISection ISGlobal = new INISection();
		ISGlobal.put("COM_INFO", " Przyk³adowy plik konfiguracyjny w formacie INI.");
		ISGlobal.put("max_enemy_count", "20");
		
		INISection ISHero = new INISection();
		ISHero.put("start_hp", "100");
		ISHero.put("start_mp", "100");
		ISHero.put("start_inv", "Item.Dagger,Item.Cap");
		
		INISection ISItemDag = new INISection();
		ISItemDag.put("dmg", "2");
		ISItemDag.put("max_durability", "15");
		ISItemDag.put("value", "4");
		
		INISection ISItemCap = new INISection();
		ISItemCap.put("ar", "1");
		ISItemCap.put("max_durability", "9");
		ISItemCap.put("value", "2");
		
		mainHeader.put("GLOBAL", ISGlobal);
		mainHeader.put("Hero", ISHero);
		mainHeader.put("Item.Dagger", ISItemDag);
		mainHeader.put("Item.Cap", ISItemCap);
		
		
		try {
			INIWriter.writeINI("tempfile.ini", mainHeader);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		
	}
	
}