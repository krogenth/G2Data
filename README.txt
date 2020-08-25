Grandia 2 Data reading/writing
 - Programming by krogenth: http://steamcommunity.com/profiles/76561198163774445
 - Thanks to woog for providing most data/afs/xls_data/*.bin entries: http://steamcommunity.com/profiles/76561198008094982/

What this does:

This is a C++ program(MSVC 2015) that takes MS_PARAM.BIN, SK_PARAM.BIN, PC_INIT.BIN, ITEM.BIN, TB_LVUP.BIN, TB_MAGIC.BIN, TB_SKILL.BIN, and TB_SPCL.BIN, and outputs their values in base-10 so those who hold no knowledge of hex, or don't want to deal
with hex editing to edit the files, can edit the files. After editing the files as desired, run the program again, and it will recreate the .BIN files for you inside the directory they are needed in.

How to use:

1.) Copy the data/afs/xls_data folder to where the program is(needs xls_data inside afs inside data), or more the program to the game root directory, right where Grandia2.exe is(steamapps/common/Grandia II Anniversary Edition).
2.) Back-up the data/afs/xls_data folder, in case anything gets messed up
3.) Run the program to create the .txt files for editing
4.) Edit the .txt files as you desire
5.) Re-run the program to recreate the .BIN files with the new data you entered

That's it.

Other information:
	
For those seeking information on files not handled yet(particularly \*.pvp, \*.nj, \*.dat, etc.) check out this Github repo:
https://github.com/bogglez/Ninja-Lib/tree/master/documentation

For \*.nj files(data/afs/map/*/*.chr, data/afs/model/*.dat) check out:
http://sharnoth.com/psodevwiki/format/nj

For \*.njm files(data/afs/map/*/*.chr, data/afs/map/*/*.chr2) check out:
http://sharnoth.com/psodevwiki/dreamcast/njm
