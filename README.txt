Grandia 2 Data reading/writing
 - Programming by krogenth: http://steamcommunity.com/profiles/76561198163774445
 - Thanks to woog for providing most data entries: http://steamcommunity.com/profiles/76561198008094982/

What this does:

This is a program that takes MS_PARAM.BIN, SK_PARAM.BIN, PC_INIT.BIN, and TB_LVUP.BIN and outputs their values in base-10 so those who hold no knowledge of hex, or don't want to deal
with hex editing to edit the files. After editing the files as desired, run the program again, and it will recreate the .BIN files for you inside the directory they are needed in.

How to use:

1.) Move the program to where Grandia II Anniversary Edition is installed, right where Grandia2.exe is(steamapps/common/Grandia II Anniversary Edition).
2.) Back-up the data/afs/xls_data folder, in case anything gets messed up
3.) Run the program to create the .txt files for editing
4.) Edit the .txt files as you desire
5.) Re-run the program to recreate the .BIN files with the new data you entered

That's it.

What knowledge is needed:

The main thing anyone who is using this needs to know is the data type ranges: https://msdn.microsoft.com/en-us/library/s3f49ktz.aspx
The important data ranges are:
	1 byte  = 0 to 255 OR -128 to 127
	2 bytes = 0 to 65,535 OR –32,768 to 32,767
	4 bytes = 0 to 4,294,967,295 OR –2,147,483,648 to 2,147,483,647
Please also know that ALL files have to have the same length entries(ie, burn!, burnflame, ba-boom!, etc, need to have the same byte length(108)), any more or less and the data will be
corrupted, so please make sure you are not adding or removing data from the .txt files.


How the files are detailed between several .txt files provided with this program:
	MS_PARAM_data.txt
	SK_PARAM_data.txt
	TB_LVUP_data.txt
	PC_INIT_data.txt
	magic.txt
	lvlup.txt
	pcinit.txt

Included by not used by this program yet:
	enemycompare.txt
	eskills.txt
	items.txt
	nme list.txt

What's next:

I would like to add in the enemies and items to this program as well, to make everything as easy as humanly possible for any and everyone to edit this game. If anyone has any other ideas,
please let me know, but I believe those 2 things are enough for most modders for this game.

Other information:

I learned a few things messing around with the files:
MS_PARAM.BIN:
	The game this file literally. It does not care about the ID at the start of the spell/move, it might as well not exist. However, you can safely remove any empty entries at the 
end of the file(after Taint of WInd), and the game will still work perfectly). If you try to remove the empty entries between spells/moves, then the spells/moves will be off-place
(i.e. - if you remove 1 entry before Tenseiken Slash, Ryudo will have a start of Flying Tenseiken, Purple Lightning, and Sky Dragon Slash, with the old Sky Dragon Slash being an empty entry.

SK_PARAM.BIN:
	Same thing as MS_PARAM.BIN, it's entries are literal, only trailing empty entries can be removed.

PC_INIT.BIN:
	The last 6 entries are just copies of Ryudo, all 6 can safely be removed.
