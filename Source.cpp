#include <fstream>
#include <bitset>
#include <vector>
#include <windows.h>
#include <assert.h>

void writeMS();
void readMS();
void writeSK();
void readSK();
void writeTB();
void readTB();
void writePC();
void readPC();

int main() {

	std::wstring files[4] = { L"MS_PARAM", L"SK_PARAM", L"TB_LVUP", L"PC_INIT" };
	std::wstring test, test2;
	unsigned char fileFlags = 0x00;

	for (int count = 0; count < 4; count++) {

		test = L"data/afs/xls_data/" + files[count] + L".txt";
		test2 = L"data/afs/xls_data/" + files[count] + L".BIN";
		if (GetFileAttributesW(test2.c_str()) != INVALID_FILE_ATTRIBUTES)
				fileFlags += pow(2, (count * 2) + 1);

		if (GetFileAttributesW(test.c_str()) != INVALID_FILE_ATTRIBUTES)
			fileFlags += pow(2, (count * 2));

	}

	if (fileFlags & 0x01)
		writeMS();
	else if ((fileFlags & 0x02) >> 1)
		readMS();

	if ((fileFlags & 0x04) >> 2)
		writeSK();
	else if ((fileFlags & 0x08) >> 3)
		readSK();

	if ((fileFlags & 0x10) >> 4)
		writeTB();
	else if ((fileFlags & 0x20) >> 5)
		readTB();

	if ((fileFlags & 0x40) >> 6)
		writePC();
	else if ((fileFlags & 0x80) >> 7)
		readPC();

	return EXIT_SUCCESS;

}

void writeMS() {

	unsigned _int16 data16 = 0;
	unsigned _int32 data32 = 0;
	std::string stringData;
	std::bitset<8> binary[108];

	std::ifstream input("data/afs/xls_data/MS_PARAM.txt");
	FILE* output;
	fopen_s(&output, "data/afs/xls_data/MS_PARAM.BIN", "w+b");
	assert(output);

	input.clear();
	input.seekg(0, std::ios::beg);

	if (input.is_open()) {

		while (std::getline(input, stringData)) {

			binary[0] = std::stoi(stringData.substr(0, stringData.find(' ')));
			stringData.erase(0, stringData.find(' ') + 1);

			binary[1] = std::stoi(stringData.substr(0, stringData.find(' ')));
			stringData.erase(0, stringData.find(' ') + 1);
			
			for (int count = 0; count < 18; count++)
				binary[2 + count] = stringData[count];

			stringData.erase(0, 19);

			data16 = std::stoi(stringData.substr(0, stringData.find(' ')));
			binary[20] = data16 & 0x00FF;
			binary[21] = (data16 & 0xFF00) >> 8;
			stringData.erase(0, stringData.find(' ') + 1);

			binary[22] = std::stoi(stringData.substr(0, stringData.find(' ')));
			stringData.erase(0, stringData.find(' ') + 1);
			binary[23] = std::stoi(stringData.substr(0, stringData.find(' ')));
			stringData.erase(0, stringData.find(' ') + 1);

			for (int count = 0; count < 4; count++) {

				data16 = std::stoi(stringData.substr(0, stringData.find(' ')));
				binary[24 + (count * 2)] = data16 & 0x00FF;
				binary[25 + (count * 2)] = (data16 & 0xFF00) >> 8;
				stringData.erase(0, stringData.find(' ') + 1);

			}

			for (int count = 0; count < 8; count++) {

				binary[32 + count] = std::stoi(stringData.substr(0, stringData.find(' ')));
				stringData.erase(0, stringData.find(' ') + 1);

			}

			data32 = std::stoi(stringData.substr(0, stringData.find(' ')));
			binary[40] = data32 & 0x000000FF;
			binary[41] = (data32 & 0x0000FF00) >> 8;
			binary[42] = (data32 & 0x00FF0000) >> 16;
			binary[43] = (data32 & 0xFF000000) >> 24;
			stringData.erase(0, stringData.find(' ') + 1);

			for (int count = 0; count < 12; count++) {

				binary[44 + count] = std::stoi(stringData.substr(0, stringData.find(' ')));
				stringData.erase(0, stringData.find(' ') + 1);

			}

			for (int count = 0; count < 6; count++) {

				data16 = std::stoi(stringData.substr(0, stringData.find(' ')));
				binary[56 + (count * 2)] = data16 & 0x00FF;
				binary[57 + (count * 2)] = (data16 & 0xFF00) >> 8;
				stringData.erase(0, stringData.find(' ') + 1);

			}

			for (int count = 0; count < 40; count++)
				binary[68 + count] = stringData[count];

			for (int count = 0; count < 108; count++)
				fputc(binary[count].to_ulong(), output);

		}

	}
	else
		return;

	input.close();
	fclose(output);

}

void readMS() {

	std::ifstream input("data/afs/xls_data/MS_PARAM.BIN", std::ios::binary);
	std::ofstream output("data/afs/xls_data/MS_PARAM.txt");

	std::vector<unsigned char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();

	unsigned _int32 data = 0;
	std::bitset<sizeof(unsigned char) * CHAR_BIT> binary;

	for (unsigned long count = 0; count < buffer.size() - 1; count += 108) {

		//reads and outputs spell/move number, all spell/move numbers follow the entire file, non-filled entries still count
		binary = (buffer.at(count));
		output << binary.to_ulong() << ' ';

		//reads and outputs spell/move type
		binary = (buffer.at(count + 1));
		output << binary.to_ulong() << ' ';

		//reads and outputs namespace of spell/move, total of 18 characters
		for (int counter = 0; counter < 18; counter++) {

			binary = (buffer.at(count + counter + 2));
			data = binary.to_ulong();
			output << static_cast<unsigned char>(data);

		}

		data = 0;

		//reads in MP/SP cost variable
		for (int counter = 1; counter >= 0; counter--) {

			binary = (buffer.at(count + 20 + counter));
			data = data << 8;
			data += binary.to_ulong();

		}

		//MP/SP cost variable
		output << ' ' << data << ' ';


		//first target variable
		binary = (buffer.at(count + 22));
		output << binary.to_ulong() << ' ';

		//second target variable
		binary = (buffer.at(count + 23));
		output << binary.to_ulong() << ' ';

		//reads and outputs +strength, power, range, and spell/move cast speed, in that order
		for (int counting = 0; counting <= 3; counting++) {

			data = 0;

			for (int counter = 1; counter >= 0; counter--) {

				binary = (buffer.at(count + 24 + counter + (counting * 2)));
				data = data << 8;
				data += binary.to_ulong();

			}

			output << data << ' ';

		}

		for (int counter = 0; counter <= 7; counter++) {

			binary = (buffer.at(count + 32 + counter));
			output << binary.to_ulong() << ' ';

		}

		data = count + 44;

		binary = (buffer.at(count + 43));
		data = binary.to_ulong();
		binary = (buffer.at(count + 42));
		data = data << 8;
		data += binary.to_ulong();
		binary = (buffer.at(count + 41));
		data = data << 8;
		data += binary.to_ulong();
		binary = (buffer.at(count + 40));
		data = data << 8;
		data += binary.to_ulong();

		output << data << ' ';

		for (int counter = 0; counter <= 3; counter++) {

			binary = (buffer.at(count + 44 + counter));
			output << binary.to_ulong() << ' ';

		}

		//reads and outputs status effects, % chance of status effect, attack param, defense param, act param, and mov param, special effect, in that order
		for (int counter = 0; counter <= 6; counter++) {

			binary = (buffer.at(count + 48 + counter));
			output << binary.to_ulong() << ' ';

		}

		//reads and outputs unknown value, not sure what it does
		binary = (buffer.at(count + 55));
		output << binary.to_ulong() << ' ';

		//reads and outputs level 1 to 5 cost and the level daamge multiplier, in that order
		for (int counter = 0; counter <= 5; counter++) {

			data = 0;

			binary = (buffer.at(count + 56 + (counter * 2) + 1));
			data = data << 8;
			data += binary.to_ulong();
			binary = (buffer.at(count + 56 + (counter * 2)));
			data = data << 8;
			data += binary.to_ulong();

			output << data << ' ';

		}

		//reads and outputs description of the spell/move
		for (int counter = 0; counter < 40; counter++) {

			binary = (buffer.at(count + 68 + counter));
			data = binary.to_ulong();
			output << static_cast<unsigned char>(data);

		}

		output << '\n';

	}

	output.close();

}

void writeSK() {

	unsigned _int16 data16 = 0;
	std::string stringData;
	std::bitset<8> binary[104];

	std::ifstream input("data/afs/xls_data/SK_PARAM.txt");
	FILE* output;
	fopen_s(&output, "data/afs/xls_data/SK_PARAM.BIN", "w+b");
	assert(output);

	input.clear();
	input.seekg(0, std::ios::beg);

	if (input.is_open()) {

		while (std::getline(input, stringData)) {

			for (int count = 0; count < 18; count++)
				binary[count] = stringData[count];

			stringData.erase(0, 19);

			binary[18] = std::stoi(stringData.substr(0, stringData.find(' ')));
			stringData.erase(0, stringData.find(' ') + 1);
			binary[19] = std::stoi(stringData.substr(0, stringData.find(' ')));
			stringData.erase(0, stringData.find(' ') + 1);

			for (int count = 0; count < 9; count++) {

				data16 = std::stoi(stringData.substr(0, stringData.find(' ')));
				binary[20 + (count * 2)] = data16 & 0x00FF;
				binary[21 + (count * 2)] = (data16 & 0xFF00) >> 8;
				stringData.erase(0, stringData.find(' ') + 1);

			}

			for (int count = 0; count < 14; count++) {

				binary[38] = std::stoi(stringData.substr(0, stringData.find(' ')));
				stringData.erase(0, stringData.find(' ') + 1);

			}

			for (int count = 0; count < 6; count++) {

				data16 = std::stoi(stringData.substr(0, stringData.find(' ')));
				binary[52 + (count * 2)] = data16 & 0x00FF;
				binary[53 + (count * 2)] = (data16 & 0xFF00) >> 8;
				stringData.erase(0, stringData.find(' ') + 1);

			}

			for (int count = 0; count < 40; count++)
				binary[64 + count] = stringData[count];

			for (int count = 0; count < 104; count++)
				fputc(binary[count].to_ulong(), output);

		}

	}
	else
		return;

	input.close();
	fclose(output);

}

void readSK() {

	//SK_PARAM each entry is 104 bytes long
	std::ifstream input("data/afs/xls_data/SK_PARAM.BIN", std::ios::binary);
	std::ofstream output("data/afs/xls_data/SK_PARAM.txt");

	std::vector<unsigned char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();

	unsigned _int32 data = 0;
	std::bitset<sizeof(unsigned char) * CHAR_BIT> binary;

	for (unsigned long count = 0; count < buffer.size() - 1; count += 104) {

		//reads and outputs namespace of skills, total of 18 characters
		for (int counter = 0; counter < 18; counter++) {

			binary = (buffer.at(count + counter));
			output << static_cast<unsigned char>(binary.to_ulong());

		}

		output << ' ';

		//reads in MP/SP cost variable
		for (int counter = 0; counter <= 1; counter++) {

			binary = (buffer.at(count + 18 + counter));
			output << binary.to_ulong() << ' ';

		}

		//reads and outputs HP, MP, SP, STR, VIT, ACT, MOV, MAG, MEN, in that order
		for (int counter = 0; counter <= 8; counter++) {

			for (int counting = 1; counting >= 0; counting--) {

				binary = (buffer.at(count + 20 + (counter * 2) + counting));
				data = data << 8;
				data += binary.to_ulong();

			}

			output << data << ' ';

			data = 0;

		}

		//reads and outputs FI, WI, EA, LI, BZ, WA, EX, FO, Special Effect, in that order
		for (int counter = 0; counter < 14; counter++) {

			binary = (buffer.at(count + 38 + counter));
			output << binary.to_ulong() << ' ';

		}

		//reads and outputs level 1 to 5 cost and the skill multiplier, in that order
		for (int counter = 0; counter <= 5; counter++) {

			for (int counting = 1; counting >= 0; counting--) {

				binary = (buffer.at(count + 52 + (counter * 2) + counting));
				data = data << 8;
				data += binary.to_ulong();

			}

			output << data << ' ';
			data = 0;

		}

		//reads and outputs the description of the skill
		for (int counter = 0; counter < 40; counter++) {

			binary = (buffer.at(count + counter + 64));
			output << static_cast<unsigned char>(binary.to_ulong());

		}

		output << '\n';

	}

	output.close();

}

void writeTB() {

	unsigned _int16 data16 = 0;
	unsigned _int32 data32 = 0;
	std::string stringData;
	std::bitset<8> binary[24];

	std::ifstream input("data/afs/xls_data/TB_LVUP.txt");
	FILE* output;
	fopen_s(&output, "data/afs/xls_data/TB_LVUP.BIN", "w+b");
	assert(output);

	input.clear();
	input.seekg(0, std::ios::beg);

	if (input.is_open()) {

		while (std::getline(input, stringData)) {

			data32 = std::stoi(stringData.substr(0, stringData.find(' ') + 1));
			binary[0] = data32 & 0x000000FF;
			binary[1] = (data32 & 0x0000FF00) >> 8;
			binary[2] = (data32 & 0x00FF0000) >> 16;
			binary[3] = (data32 & 0xFF000000) >> 24;
			stringData.erase(0, stringData.find(' ') + 1);

			for (int count = 0; count < 10; count++) {

				data16 = std::stoi(stringData.substr(0, stringData.find(' ') + 1));
				binary[4 + (count * 2)] = data16 & 0x00FF;
				binary[5 + (count * 2)] = (data16 & 0xFF00) >> 8;
				stringData.erase(0, stringData.find(' ') + 1);

			}

			for (int count = 0; count < 24; count++)
				fputc(binary[count].to_ulong(), output);

		}

	}
	else
		return;

	input.close();
	fclose(output);

}

void readTB() {

	// TB_LVUP each entry is 24 bytes long
	std::ifstream input("data/afs/xls_data/TB_LVUP.BIN", std::ios::binary);
	std::ofstream output("data/afs/xls_data/TB_LVUP.txt");

	std::vector<unsigned char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();

	unsigned _int32 data;
	std::bitset<sizeof(unsigned char) * CHAR_BIT> binary;

	for (int count = 0; count < buffer.size() - 1; count += 24) {

		data = 0;

		//reads and outputs EXP for each level up
		for (int counter = 3; counter >= 0; counter--) {

			binary = (buffer.at(count + counter));
			data = data << 8;
			data += binary.to_ulong();

		}

		output << data << ' ';
		data = 0;

		//reads and outputs HP, MP, SP, STR, VIT, MAG, MEN, ACT, MOV, gained from each level up, in that order
		for (int counter = 0; counter < 10; counter++) {

			for (int counting = 1; counting >= 0; counting--) {

				binary = (buffer.at(count + (counter * 2) + counting + 4));
				data = data << 8;
				data += binary.to_ulong();

			}

			output << data << ' ';
			data = 0;

		}

		output << '\n';

	}

	output.close();

}

void writePC() {

	unsigned _int32 data32 = 0;
	std::string stringData;
	std::bitset<8> binary[80];

	std::ifstream input("data/afs/xls_data/PC_INIT.txt");
	FILE* output;
	fopen_s(&output, "data/afs/xls_data/PC_INIT.BIN", "w+b");
	assert(output);

	input.clear();
	input.seekg(0, std::ios::beg);

	if (input.is_open()) {

		while (std::getline(input, stringData)) {

			data32 = std::stoi(stringData.substr(0, stringData.find(' ') + 1));
			binary[0] = data32 & 0x000000FF;
			binary[1] = (data32 & 0x0000FF00) >> 8;
			binary[2] = (data32 & 0x00FF0000) >> 16;
			binary[3] = (data32 & 0xFF000000) >> 24;
			stringData.erase(0, stringData.find(' ') + 1);

			for (int count = 0; count < 76; count++) {

				binary[4 + count] = std::stoi(stringData.substr(0, stringData.find(' ') + 1));
				stringData.erase(0, stringData.find(' ') + 1);

			}

			for (int count = 0; count < 80; count++)
				fputc(binary[count].to_ulong(), output);

		}

	}
	else
		return;

	input.close();
	fclose(output);

}

void readPC() {

	//PC_INIT each entry is 80 bytes long
	std::ifstream input("data/afs/xls_data/PC_INIT.BIN", std::ios::binary);
	std::ofstream output("data/afs/xls_data/PC_INIT.txt");

	std::vector<unsigned char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();

	unsigned _int32 data;
	std::bitset<sizeof(unsigned char) * CHAR_BIT> binary;

	for (int count = 0; count < buffer.size() - 1; count += 80) {

		data = 0;

		//reads and outputs EXP value of start for character
		for (int counter = 3; counter >= 0; counter--) {

			binary = (buffer.at(count + counter));
			data = data << 8;
			data += binary.to_ulong();

		}

		output << data << ' ';

		//reads and outputs mostly data I don't know the purpose of(if anyone figures this shit out, let me know)
		for (int counter = 0; counter < 76; counter++) {

			binary = (buffer.at(count + counter + 4));
			output << binary.to_ulong() << ' ';

		}

		output << '\n';

	}

	output.close();

}
