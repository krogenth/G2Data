#include <fstream>
#include <bitset>
#include <vector>
#include <windows.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cinttypes>

void writeMS();
void readMS();
void writeSK();
void readSK();
void writeTB();
void readTB();
void writePC();
void readPC();
void writeIT();
void readIT();

int main() {

	std::wstring files[5] = { L"MS_PARAM", L"SK_PARAM", L"TB_LVUP", L"PC_INIT", L"ITEM" };
	std::wstring test, test2;
	unsigned _int16 fileFlags = 0;

	for (int count = 0; count < 5; count++) {

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

	if ((fileFlags & 0x100) >> 8)
		writeIT();
	else if ((fileFlags & 0x200) >> 9)
		readIT();

	return 0;

}

void writeMS() {

	uint8_t offset = 0;
	uint16_t data16 = 0;
	uint32_t data32 = 0;
	std::string data;
	std::bitset<8> binary[108];

	std::ifstream input("data/afs/xls_data/MS_PARAM.txt");
	FILE* output;
	fopen_s(&output, "data/afs/xls_data/MS_PARAM.BIN", "w+b");
	assert(output);

	input.clear();
	input.seekg(0, std::ios::beg);

	if (input.is_open()) {

		while (std::getline(input, data)) {

			while (data[offset] == ' ')
				offset++;

			//ID/offset
			binary[0] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
			offset = data.find(' ', static_cast<size_t>(offset));

			while (data[offset] == ' ')
				offset++;

			//Icon
			binary[1] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
			offset = data.find(' ', static_cast<size_t>(offset));
			
			//Name
			for (int count = 0; count < 18; count++)
				binary[2 + count] = data[offset + count + 1];

			data.erase(0, offset + 19);
			offset = 0;

			while (data[offset] == ' ')
				offset++;

			//Cost
			data16 = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
			binary[20] = data16 & 0x00FF;
			binary[21] = (data16 & 0xFF00) >> 8;
			offset = data.find(' ', static_cast<size_t>(offset));

			while (data[offset] == ' ')
				offset++;

			//Target Effect
			binary[22] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
			offset = data.find(' ', static_cast<size_t>(offset));

			while (data[offset] == ' ')
				offset++;

			//Target Type
			binary[23] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
			offset = data.find(' ', static_cast<size_t>(offset));

			//strength, power, range, cast lv1, cast lv5, recovery, animation
			for (int count = 0; count < 7; count++) {

				while (data[offset] == ' ')
					offset++;

				data16 = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				binary[24 + (count * 2)] = data16 & 0x00FF;
				binary[25 + (count * 2)] = (data16 & 0xFF00) >> 8;
				offset = data.find(' ', static_cast<size_t>(offset));

			}

			//unknown 2 bytes
			for (int count = 0; count < 2; count++) {

				while (data[offset] == ' ')
					offset++;

				binary[38 + count] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				offset = data.find(' ', static_cast<size_t>(offset));

			}

			//IP Damage, IP Cancel Damage, Knockback
			for (int count = 0; count < 3; count++) {

				while (data[offset] == ' ')
					offset++;

				data16 = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				binary[40 + (count * 2)] = data16 & 0x00FF;
				binary[41 + (count * 2)] = (data16 & 0xFF00) >> 8;
				offset = data.find(' ', static_cast<size_t>(offset));

			}

			//element, elem strength, status effects, % chance effect, strength, defense, action, movement
			for (int count = 0; count < 8; count++) {

				while (data[offset] == ' ')
					offset++;

				binary[46 + count] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				offset = data.find(' ', static_cast<size_t>(offset));

			}

			//special effect, lv1 - 5, mlt
			for (int count = 0; count < 7; count++) {

				while (data[offset] == ' ')
					offset++;

				data16 = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				binary[54 + (count * 2)] = data16 & 0x00FF;
				binary[55 + (count * 2)] = (data16 & 0xFF00) >> 8;
				offset = data.find(' ', static_cast<size_t>(offset));

			}

			for (int count = 0; count < 40; count++)
				binary[68 + count] = data[offset + count];

			for (int count = 0; count < 108; count++)
				fputc(binary[count].to_ulong(), output);

			offset = 0;

		}

	}
	else
		return;

	input.close();
	fclose(output);

}

void readMS() {

	uint32_t data = 0;
	std::bitset<sizeof(unsigned char) * CHAR_BIT> binary;

	std::ifstream input("data/afs/xls_data/MS_PARAM.BIN", std::ios::binary);
	std::ofstream output("data/afs/xls_data/MS_PARAM.txt");

	std::vector<unsigned char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();

	for (unsigned long count = 0; count < buffer.size() - 1; count += 108) {

		//reads and outputs spell/move number, all spell/move numbers follow the entire file, non-filled entries still count
		output << std::setw(3) << std::to_string(buffer.at(count)) << ' ';

		//reads and outputs Icon
		output << std::setw(2) << std::to_string(buffer.at(count + 1)) << ' ';

		//reads and outputs namespace of spell/move, total of 18 characters
		for (int counter = 0; counter < 18; counter++)
			output << buffer.at(count + counter + 2);

		output << ' ';

		//reads in MP/SP cost variable
		for (int counter = 1; counter >= 0; counter--) {

			binary = (buffer.at(count + 20 + counter));
			data = data << 8;
			data += binary.to_ulong();

		}

		output << std::setw(2) << data << ' ';
		data = 0;

		//first target variable
		output << std::setw(2) << std::to_string(buffer.at(count + 22)) << ' ';

		//second target variable
		output << std::setw(2) << std::to_string(buffer.at(count + 23)) << ' ';

		//reads and outputs strength, power, range, cast speed lv1, cast speed lv5, recovery time, animation
		for (int counting = 0; counting < 7; counting++) {

			for (int counter = 1; counter >= 0; counter--) {

				binary = (buffer.at(count + 24 + counter + (counting * 2)));
				data = data << 8;
				data += binary.to_ulong();

			}

			output << std::setw(5) << data << ' ';
			data = 0;

		}

		//read and output unknown data
		for (int counter = 0; counter < 2; counter++) {

			binary = (buffer.at(count + 38 + counter));
			output << binary.to_ulong() << ' ';

		}

		//read and output IP Damage, IP Cancel Damage, Knockback
		for (int counting = 0; counting < 3; counting++) {

			for (int counter = 1; counter >= 0; counter--) {

				binary = (buffer.at(count + 40 + counter + (counting * 2)));
				data = data << 8;
				data += binary.to_ulong();

			}

			output << std::setw(4) << data << ' ';
			data = 0;

		}

		//reads and outputs element, element strength, status effects, % chance of status effect
		for (int counter = 0; counter < 4; counter++)
			output << std::setw(3) << std::to_string(buffer.at(count + 46 + counter)) << ' ';

		//reads and outputs attack, defense, act, mov modifiers
		for (int counter = 0; counter < 4; counter++)
			output << std::setw(3) << std::to_string(static_cast<char>(buffer.at(count + 50 + counter))) << ' ';

		//special effect
		for (int counter = 1; counter >= 0; counter--) {

			binary = (buffer.at(count + 54 + counter));
			data = data << 8;
			data += binary.to_ulong();

		}

		output << std::setw(4) << data << ' ';
		data = 0;

		//reads and outputs level 1 to 5 cost and the level damage multiplier, in that order
		for (int counter = 0; counter < 6; counter++) {

			data = 0;

			binary = (buffer.at(count + 56 + (counter * 2) + 1));
			data = data << 8;
			data += binary.to_ulong();
			binary = (buffer.at(count + 56 + (counter * 2)));
			data = data << 8;
			data += binary.to_ulong();

			output << std::setw(4) << data << ' ';

		}

		data = 0;

		//reads and outputs description of the spell/move
		for (int counter = 0; counter < 40; counter++)
			output << buffer.at(count + 68 + counter);

		output << '\n';

	}

	output.close();

}

void writeSK() {

	uint8_t offset = 0;
	uint16_t data16 = 0;
	std::string data;
	std::bitset<8> binary[104];

	std::ifstream input("data/afs/xls_data/SK_PARAM.txt");
	FILE* output;
	fopen_s(&output, "data/afs/xls_data/SK_PARAM.BIN", "w+b");
	assert(output);

	input.clear();
	input.seekg(0, std::ios::beg);

	if (input.is_open()) {

		while (std::getline(input, data)) {

			for (int count = 0; count < 18; count++)
				binary[count] = data[count];

			data.erase(0, 19);

			while (data[offset] == ' ')
				offset++;

			binary[18] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
			offset = data.find(' ', static_cast<size_t>(offset));

			while (data[offset] == ' ')
				offset++;

			binary[19] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
			offset = data.find(' ', static_cast<size_t>(offset));

			for (int count = 0; count < 9; count++) {

				while (data[offset] == ' ')
					offset++;

				data16 = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				offset = data.find(' ', static_cast<size_t>(offset));
				binary[20 + (count * 2)] = data16 & 0x00FF;
				binary[21 + (count * 2)] = (data16 & 0xFF00) >> 8;

			}

			for (int count = 0; count < 14; count++) {

				while (data[offset] == ' ')
					offset++;

				binary[38 + count] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				offset = data.find(' ', static_cast<size_t>(offset));

			}

			for (int count = 0; count < 6; count++) {

				while (data[offset] == ' ')
					offset++;

				data16 = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				offset = data.find(' ', static_cast<size_t>(offset));
				binary[52 + (count * 2)] = data16 & 0x00FF;
				binary[53 + (count * 2)] = (data16 & 0xFF00) >> 8;

			}

			for (int count = 0; count < 40; count++)
				binary[64 + count] = data[offset + count + 1];

			for (int count = 0; count < 104; count++)
				fputc(binary[count].to_ulong(), output);

			offset = 0;

		}

	}
	else
		return;

	input.close();
	fclose(output);

}

void readSK() {

	unsigned _int32 data = 0;
	std::bitset<sizeof(unsigned char) * CHAR_BIT> binary;

	//SK_PARAM each entry is 104 bytes long
	std::ifstream input("data/afs/xls_data/SK_PARAM.BIN", std::ios::binary);
	std::ofstream output("data/afs/xls_data/SK_PARAM.txt");

	std::vector<unsigned char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();

	for (unsigned long count = 0; count < buffer.size() - 1; count += 104) {

		//reads and outputs namespace of skills, total of 18 characters
		for (int counter = 0; counter < 18; counter++)
			output << buffer.at(count + counter);

		output << ' ';

		//reads in MP/SP cost variable
		for (int counter = 0; counter <= 1; counter++)
			output << std::to_string(buffer.at(count + 18 + counter)) << ' ';

		//reads and outputs HP, MP, SP, STR, VIT, ACT, MOV, MAG, MEN
		for (int counter = 0; counter <= 8; counter++) {

			for (int counting = 1; counting >= 0; counting--) {

				binary = (buffer.at(count + 20 + (counter * 2) + counting));
				data = data << 8;
				data += binary.to_ulong();

			}

			output << std::setw(2) << data << ' ';

			data = 0;

		}

		//reads and outputs FI, WI, EA, LI, BZ, WA, EX, FO, Special Effect
		for (int counter = 0; counter < 14; counter++)
			output << std::setw(2) << std::to_string(buffer.at(count + 38 + counter)) << ' ';

		//reads and outputs level 1 to 5 cost and the skill multiplier
		for (int counter = 0; counter <= 5; counter++) {

			for (int counting = 1; counting >= 0; counting--) {

				binary = (buffer.at(count + 52 + (counter * 2) + counting));
				data = data << 8;
				data += binary.to_ulong();

			}

			output << std::setw(4) << data << ' ';
			data = 0;

		}

		//reads and outputs the description of the skill
		for (int counter = 0; counter < 40; counter++)
			output << buffer.at(count + counter + 64);

		output << '\n';

	}

	output.close();

}

void writeTB() {

	uint8_t offset = 0;
	uint16_t data16 = 0;
	uint32_t data32 = 0;
	std::string data;
	std::bitset<8> binary[24];

	std::ifstream input("data/afs/xls_data/TB_LVUP.txt");
	FILE* output;
	fopen_s(&output, "data/afs/xls_data/TB_LVUP.BIN", "w+b");
	assert(output);

	input.clear();
	input.seekg(0, std::ios::beg);

	if (input.is_open()) {

		while (std::getline(input, data)) {

			while (data[offset] == ' ')
				offset++;

			data32 = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
			offset = data.find(' ', static_cast<size_t>(offset));
			binary[0] = data32 & 0x000000FF;
			binary[1] = (data32 & 0x0000FF00) >> 8;
			binary[2] = (data32 & 0x00FF0000) >> 16;
			binary[3] = (data32 & 0xFF000000) >> 24;

			for (int count = 0; count < 10; count++) {

				while (data[offset] == ' ')
					offset++;

				data16 = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				offset = data.find(' ', static_cast<size_t>(offset));
				binary[4 + (count * 2)] = data16 & 0x00FF;
				binary[5 + (count * 2)] = (data16 & 0xFF00) >> 8;

			}

			for (int count = 0; count < 24; count++)
				fputc(binary[count].to_ulong(), output);

			offset = 0;

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

		output << std::setw(7) << data << ' ';
		data = 0;

		//reads and outputs HP, MP, SP, STR, VIT, MAG, MEN, ACT, MOV, gained from each level up, in that order
		for (int counter = 0; counter < 10; counter++) {

			for (int counting = 1; counting >= 0; counting--) {

				binary = (buffer.at(count + (counter * 2) + counting + 4));
				data = data << 8;
				data += binary.to_ulong();

			}

			output << std::setw(3) << data << ' ';
			data = 0;

		}

		output << '\n';

	}

	output.close();

}

void writePC() {

	std::ifstream input("data/afs/xls_data/PC_INIT.txt");
	FILE* output;
	fopen_s(&output, "data/afs/xls_data/PC_INIT.BIN", "w+b");
	assert(output);

	uint32_t temp = 0;
	uint16_t offset = 0;
	std::string data;
	std::bitset<8> binary[80];

	input.clear();
	input.seekg(0, std::ios::beg);

	if (input.is_open()) {

		while (std::getline(input, data)) {

			//Character Starting EXP
			while (data[offset] == ' ')
				offset++;

			temp = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
			binary[0] = temp & 0x000000FF;
			binary[1] = (temp & 0x0000FF00) >> 8;
			binary[2] = (temp & 0x00FF0000) >> 16;
			binary[3] = (temp & 0xFF000000) >> 24;

			offset = data.find(' ', static_cast<size_t>(offset));

			//Character Starting Gear
			for (uint8_t count = 0; count < 0x06; count++) {

				while (data[offset] == ' ')
					offset++;

				temp = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));

				binary[(count * 2) + 4 + 0] = temp & 0x00FF;
				binary[(count * 2) + 4 + 1] = (temp & 0xFF00) >> 8;

				offset = data.find(' ', static_cast<size_t>(offset));


				temp = 0;

			}

			//Unknown 4 byte data
			while (data[offset] == ' ')
				offset++;

			temp = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));

			binary[16 + 0] = temp & 0x000000FF;
			binary[16 + 1] = (temp & 0x0000FF00) >> 8;
			binary[16 + 2] = (temp & 0x00FF0000) >> 16;
			binary[16 + 3] = (temp & 0xFF000000) >> 24;

			offset = data.find(' ', static_cast<size_t>(offset));
			temp = 0;

			//Max HP(?), Max MP(?), Max SP(?), Max STR(?), Max VIT(?), Max AGI(?), Max SPD(?), Max MAG(?), Max MEN(?), IPS, IPCS
			for (uint8_t count = 0; count < 0x0B; count++) {

				while (data[offset] == ' ')
					offset++;

				temp = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));

				binary[(count * 2) + 20 + 0] = temp & 0x00FF;
				binary[(count * 2) + 20 + 1] = (temp & 0xFF00) >> 8;

				offset = data.find(' ', static_cast<size_t>(offset));


				temp = 0;

			}

			//Some other junk I do not know what it does, besides a couple bytes
			for (int count = 0; count < 38; count++) {

				while (data[offset] == ' ')
					offset++;

				binary[42 + count] = std::stoi(data.substr(static_cast<size_t>(offset), data.find(' ', static_cast<size_t>(offset)) - offset));
				offset = data.find(' ', static_cast<size_t>(offset));

			}

			for (int count = 0; count < 80; count++)
				fputc(binary[count].to_ulong(), output);

			offset = 0;

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

	int32_t data = 0;
	int32_t temp = 0;
	std::bitset<sizeof(unsigned char) * CHAR_BIT> binary;

	for (int count = 0; count < buffer.size() - 1; count += 80) {

		data = 0;

		//reads and outputs EXP value of start for character
		for (int counter = 3; counter >= 0; counter--) {

			binary = (buffer.at(count + counter));
			data = data << 8;
			data += binary.to_ulong();

		}

		output << std::setw(4) << data << ' ';

		//Starting Gear for Character
		for (uint8_t counter = 0; counter < 6; counter++) {
			for (int8_t counting = 1; counting >= 0; counting--) {
				binary = (buffer.at(count + (counter * 2) + counting + 4));
				temp = temp << 8;
				temp += binary.to_ulong();
			}

			output << std::setw(3) << std::to_string(static_cast<short>(temp)) << ' ';
			temp = 0;

		}

		//Unknown 4 byte data
		for (uint8_t counter = 0; counter < 1; counter++) {
			for (int8_t counting = 3; counting >= 0; counting--) {
				binary = (buffer.at(count + (counter * 2) + counting + 16));
				temp = temp << 8;
				temp += binary.to_ulong();
			}

			output << std::setw(3) << std::to_string(static_cast<short>(temp)) << ' ';
			temp = 0;

		}

		//Max HP(?), Max MP(?), Max SP(?), Max STR(?), Max VIT(?), Max AGI(?), Max SPD(?), Max MAG(?), Max MEN(?), IPS, IPCS
		for (uint8_t counter = 0; counter < 11; counter++) {
			for (int8_t counting = 1; counting >= 0; counting--) {
				binary = (buffer.at(count + (counter * 2) + counting + 20));
				temp = temp << 8;
				temp += binary.to_ulong();
			}

			output << std::setw(3) << std::to_string(static_cast<short>(temp)) << ' ';
			temp = 0;

		}

		//reads and outputs mostly data I don't know the purpose of(if anyone figures this shit out, let me know)
		for (int counter = 0; counter < 38; counter++) {

			binary = (buffer.at(count + counter + 42));
			output << std::setw(3) << binary.to_ulong() << ' ';

		}

		output << '\n';

	}

	output.close();

}

void writeIT() {

	std::ifstream input("data/afs/xls_data/ITEM.txt");
	FILE* output;
	fopen_s(&output, "data/afs/xls_data/ITEM.BIN", "w+b");
	assert(output);

	uint32_t eIndex = 0;
	uint32_t iIndex = 0;
	uint32_t temp = 0;
	uint8_t offset = 0;
	uint8_t entryLength = 0x50;
	uint16_t entryCount = 0;
	std::vector<unsigned char> buffer(0x12A48, 0);
	std::string data = "";
	std::string item[3] = {};

	while (std::getline(input, data)) {

		uint8_t delimCount = std::count(data.begin(), data.end(), ':');
		delimCount = (delimCount + std::count(data.begin(), data.end(), ';')) / 2;
		for (uint8_t count = 0; count < delimCount; count++) {

			item[count] = data.substr(0, data.find(":;"));
			data.erase(0, data.find(":;") + 2);

		}

		//Read and output Name
		for (uint8_t count = 0; count < 0x12; count++)
			buffer[entryCount * entryLength + count] = item[0][0x00 + count];

		//Read and output Description
		for (uint8_t count = 0; count < 0x28; count++)
			buffer[entryCount * entryLength + count + 0x12] = item[0][0x13 + count];

		item[0].erase(0x00, 0x3B);

		//Read and output Item Type, ?, ?, ?, ?, ?
		for (uint8_t count = 0; count < 0x06; count++) {

			while (item[0][offset] == ' ')
				offset++;

			temp = std::stoi(item[0].substr(static_cast<size_t>(offset), item[0].find(' ', static_cast<size_t>(offset)) - offset));
			offset = item[0].find(' ', static_cast<size_t>(offset));

			buffer[entryCount * entryLength + count + 0x3A] = temp & 0xFF;
			temp = 0;

		}

		//Read and output Price, eIndex, iIndex, ID
		for (uint8_t count = 0; count < 0x04; count++) {

			while (item[0][offset] == ' ')
				offset++;

			temp = std::stoi(item[0].substr(static_cast<size_t>(offset), item[0].find(' ', static_cast<size_t>(offset)) - offset));
			offset = item[0].find(' ', static_cast<size_t>(offset));
			
			if (count == 1)
				eIndex = temp;
			else if (count == 2)
				iIndex = temp;

			for (uint8_t counter = 0; counter < 4; counter++)
			{
				buffer[entryCount * entryLength + (count * 4) + counter + 0x40] = temp & 0xFF;
				temp = temp >> 8;
			}

			temp = 0;

		}

		offset = 0;
		uint8_t itemOffset = 0;

		//Check if it is Equipment
		if (eIndex != 0xFFFFFFFF) {

			itemOffset++;

			//Character bitflag, Strength, Defense, Action, Movement
			for (uint8_t count = 0; count < 0x05; count++) {

				while (item[1][offset] == ' ')
					offset++;

				temp = std::stoi(item[1].substr(static_cast<size_t>(offset), item[1].find(' ', static_cast<size_t>(offset)) - offset));
				offset = item[1].find(' ', static_cast<size_t>(offset));

				for (uint8_t counter = 0; counter < 2; counter++)
				{
					buffer[eIndex + (count * 2) + counter] = temp & 0xFF;
					temp = temp >> 8;
				}

				temp = 0;

			}

			//EF, FI, WI, EA, LT, BZ, ST, %, ?, ?, ?, ?, ?, ?, ?, ?
			for (uint8_t count = 0; count < 0x10; count++) {

				while (item[1][offset] == ' ')
					offset++;

				temp = std::stoi(item[1].substr(static_cast<size_t>(offset), item[1].find(' ', static_cast<size_t>(offset)) - offset));
				offset = item[1].find(' ', static_cast<size_t>(offset));

				buffer[eIndex + count + 0x0A] = temp & 0xFF;
				temp = 0;

			}

			//SP
			while (item[1][offset] == ' ')
				offset++;

			temp = std::stoi(item[1].substr(static_cast<size_t>(offset), item[1].find(' ', static_cast<size_t>(offset)) - offset));
			offset = item[1].find(' ', static_cast<size_t>(offset));

			for (uint8_t counter = 0; counter < 2; counter++)
			{
				buffer[eIndex + counter+ 0x1A] = temp & 0xFF;
				temp = temp >> 8;
			}

			temp = 0;

		}

		offset = 0;

		//Check if it is an Item
		if (iIndex != 0xFFFFFFFF) {

			//Target Effect, Target Type
			for (uint8_t count = 0; count < 0x02; count++) {

				while (item[1 + itemOffset][offset] == ' ')
					offset++;

				temp = std::stoi(item[1 + itemOffset].substr(static_cast<size_t>(offset), item[1 + itemOffset].find(' ', static_cast<size_t>(offset)) - offset));
				offset = item[1 + itemOffset].find(' ', static_cast<size_t>(offset));

				buffer[iIndex + count] = temp & 0xFF;
				temp = 0;

			}

			//Power, Range, Cast Time, Recovery, Animation
			for (uint8_t count = 0; count < 0x05; count++) {

				while (item[1 + itemOffset][offset] == ' ')
					offset++;

				temp = std::stoi(item[1 + itemOffset].substr(static_cast<size_t>(offset), item[1 + itemOffset].find(' ', static_cast<size_t>(offset)) - offset));
				offset = item[1 + itemOffset].find(' ', static_cast<size_t>(offset));

				for (uint8_t counter = 0; counter < 2; counter++)
				{
					buffer[iIndex + (count * 2) + counter + 0x02] = temp & 0xFF;
					temp = temp >> 8;
				}

				temp = 0;
			}

			//Effective against monster type, ?
			for (uint8_t count = 0; count < 0x02; count++) {

				while (item[1 + itemOffset][offset] == ' ')
					offset++;

				temp = std::stoi(item[1 + itemOffset].substr(static_cast<size_t>(offset), item[1 + itemOffset].find(' ', static_cast<size_t>(offset)) - offset));
				offset = item[1 + itemOffset].find(' ', static_cast<size_t>(offset));

				buffer[iIndex + count + 0x0C] = temp & 0xFF;
				temp = 0;

			}

			//IP Damage, IP Cancel Damage, Knockback
			for (uint8_t count = 0; count < 0x03; count++) {

				while (item[1 + itemOffset][offset] == ' ')
					offset++;

				temp = std::stoi(item[1 + itemOffset].substr(static_cast<size_t>(offset), item[1 + itemOffset].find(' ', static_cast<size_t>(offset)) - offset));
				offset = item[1 + itemOffset].find(' ', static_cast<size_t>(offset));

				for (uint8_t counter = 0; counter < 2; counter++)
				{
					buffer[iIndex + (count * 2) + counter + 0x0E] = temp & 0xFF;
					temp = temp >> 8;
				}

				temp = 0;
			}

			//Element, Elem Strength, Status Effect, % Chance Effect, Strength, Defense, Action, Movement, % Chance break, ?, ?, ?
			for (uint8_t count = 0; count < 0x0C; count++) {

				while (item[1 + itemOffset][offset] == ' ')
					offset++;

				temp = std::stoi(item[1 + itemOffset].substr(static_cast<size_t>(offset), item[1 + itemOffset].find(' ', static_cast<size_t>(offset)) - offset));
				offset = item[1 + itemOffset].find(' ', static_cast<size_t>(offset));

				buffer[iIndex + count + 0x14] = temp & 0xFF;
				temp = 0;

			}

		}
		
		itemOffset = 0;
		offset = 0;
		entryCount++;

	}

	for (auto var : buffer)
		fputc(var, output);

	fclose(output);

}

void readIT() {

	std::ifstream input("data/afs/xls_data/ITEM.BIN", std::ios::binary);
	std::ofstream output("data/afs/xls_data/ITEM.txt");

	std::vector<unsigned char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();

	int32_t eIndex = 0;
	int32_t iIndex = 0;
	int32_t temp = 0;

	uint8_t entryIndex = 0;
	uint8_t entryLength = 0x50;


	std::bitset<sizeof(unsigned char) *CHAR_BIT> binary;

	for (uint16_t count = 0; count < 799; count++) {

		entryIndex = 0;

		//Reads and outputs the name of the item
		for (uint8_t counter = 0; counter < 0x12; counter++) {
			if (buffer[count * entryLength + entryIndex + counter] < 0x20)
				output << ' ';
			else
				output << buffer[count * entryLength + counter];
		}

		output << ' ';
		entryIndex += 0x12;

		//Reads and outputs the description of the item(not all item descriptions are filled with ' '(0x20), so if it isn't a space, make it one)
		for (uint8_t counter = 0; counter < 0x28; counter++) {

			if (buffer[count * entryLength + entryIndex + counter] < 0x20)
				output << ' ';
			else
				output << buffer[count * entryLength + entryIndex + counter];

		}

		output << ' ';
		entryIndex += 0x28;

		//Entry Type, ?, ?, ?, ?, ?
		for (uint8_t counter = 0; counter < 0x06; counter++) {

			binary = buffer[count * entryLength + entryIndex + counter];
			output << std::setw(3) << binary.to_ulong() << ' ';

		}

		output << ' ';
		entryIndex += 0x06;

		//Read and output the purchase price of the entry
		for (int8_t counter = 3; counter >= 0; counter--) {

			binary = (buffer[count * entryLength + entryIndex + counter]);
			temp = temp << 8;
			temp += binary.to_ulong();

		}

		output << ' ' << std::setw(10) << temp;
		temp = 0;
		entryIndex += 0x04;

		//ALL ITEMS HAVE AN EQUIPMENT AND ITEM OFFSET, WHICHEVER ISN'T USED IS 0XFFFFFFFF
		//Read and output the equipment offset of the item
		for (int8_t counter = 3; counter >= 0; counter--) {

			binary = (buffer[count * entryLength + entryIndex + counter]);
			eIndex = eIndex << 8;
			eIndex += binary.to_ulong();

		}

		output << ' ' << std::setw(10) << eIndex;
		entryIndex += 0x04;

		//Reads and outputs the item offset of the item
		for (int8_t counter = 3; counter >= 0; counter--) {

			binary = (buffer[count * entryLength + entryIndex + counter]);
			iIndex = iIndex << 8;
			iIndex += binary.to_ulong();

		}

		output << ' ' << std::setw(10) << iIndex;

		//Outputs the current item count, last 4 bytes were believed to be an ID, but this is not true, Grandia 2 uses the entry offset from the start of the file, for enemy drops and maps
		output << ' ' << std::setw(3) << count << ' ';

		output << ":;";

		//28 bytes
		if (eIndex != 0xFFFFFFFF) {

			uint8_t eEntryIndex = 0;

			//Character bitflag, Strength, Defense, Action, Movement
			for (uint8_t counter = 0; counter < 5; counter++) {
				for (int8_t counting = 1; counting >= 0; counting--) {
					binary = (buffer[eIndex + (counter * 2) + counting]);
					temp = temp << 8;
					temp += binary.to_ulong();
				}

				output << std::setw(3) << std::to_string(static_cast<short>(temp)) << ' ';
				temp = 0;

			}

			eEntryIndex += 0x0A;

			//EF, FI, WI, EA, LT, BZ, ST, %, ?, ?, ?, ?, ?, ?, ?, ?
			for (uint8_t counter = 0; counter < 0x10; counter++)
				output << std::setw(3) << std::to_string(buffer[eIndex + eEntryIndex + counter]) << ' ';

			eEntryIndex += 0x10;

			//SP
			for (int8_t counting = 1; counting >= 0; counting--) {
				binary = (buffer[eIndex + eEntryIndex + counting]);
				temp = temp << 8;
				temp += binary.to_ulong();
			}

			output << std::setw(3) << std::to_string(temp) << ' ';
			temp = 0;

			output << ":;";

		}


		if (iIndex != 0xFFFFFFFF) {

			uint8_t iEntryIndex = 0x02;

			//Target Effect, Target Type
			output << std::setw(2) << std::to_string(buffer[iIndex]) << ' ';
			output << std::setw(2) << std::to_string(buffer[iIndex + 1]) << ' ';

			//Power, Range, Cast Time, Recovery, Animation
			for (uint8_t counter = 0; counter < 5; counter++) {
				for (int8_t counting = 1; counting >= 0; counting--) {
					binary = (buffer[iIndex + iEntryIndex + (counter * 2) + counting]);
					temp = temp << 8;
					temp += binary.to_ulong();
				}

				output << std::setw(5) << std::to_string(temp) << ' ';
				temp = 0;
			}

			iEntryIndex += 0x0A;

			//Effective against monster type, ?
			for (uint8_t counter = 0; counter < 0x02; counter++)
				output << std::setw(3) << std::to_string(buffer[iIndex + iEntryIndex + counter]) << ' ';

			iEntryIndex += 0x02;

			//IP Damage, IP Cancel Damage, Knockback
			for (uint8_t counter = 0; counter < 3; counter++) {
				for (int8_t counting = 1; counting >= 0; counting--) {
					binary = (buffer[iIndex + iEntryIndex + (counter * 2) + counting]);
					temp = temp << 8;
					temp += binary.to_ulong();
				}

				output << std::setw(5) << std::to_string(temp) << ' ';
				temp = 0;
			}

			iEntryIndex += 0x06;

			//Element, Elem Strength, Status Effect, % Chance Effect
			for (uint8_t counter = 0; counter < 0x04; counter++)
				output << std::setw(3) << std::to_string(buffer[iIndex + iEntryIndex + counter]) << ' ';

			iEntryIndex += 0x04;

			//Strength, Defense, Action, Movement, % Chance break, ?, ?, ?
			for (uint8_t counter = 0; counter < 0x08; counter++)
				output << std::setw(3) << std::to_string(static_cast<char>(buffer[iIndex + iEntryIndex + counter])) << ' ';

			output << ":;";

		}

		output << '\n';

	}

	output.close();

}