#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <string>
#include <vector>
using namespace std;


int regs[32] = { 0 };
char memory[8 * 1024];
int pc = 0;

struct instWord
{
	string instText;
	string binarymachinecode;
	string hexmachinecode;
	int instMachineCode;
	int rd, rs1, rs2, opcode;
	int I_imm, S_imm, B_imm, U_imm, J_imm;
	string  funct3, funct7;
	string type;
	int mcase;
	string function;
};

struct label
{
	int address;
	string name;
};

struct instruction_set
{
	string name;
	string type;
	string func3;
	string func7;
	int m_case;
};

//prototypes
string myfunc(string &kamel, char begin, char end);
string add_binary(long long a, int b);
string toBinary(int n, int n_bits);
string parse(instWord &w);
void changeMachine(instWord& w);
void simulate(instWord& w);
int convertBinaryToDecimal(long long n);
string decimalToHex(int num);
string BinarytoHex32(string Binary);
bool IsNumericDigit(char C);
int HexToDec(char *hexstr, int size);

instruction_set* instructions_array = new instruction_set[37];
vector<label> labels;
vector<string> MachineCodeVector;

void emitError(char *s)
{
	cout << s;
	exit(0);
}

void printPrefix(unsigned int instA, unsigned int instCode)
{
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << hex << std::setw(8) << instCode;
}

//void instAssembleExec(instWord& inst)
//{
//	//Generate instruction machine code
//
//	//execute instruction
//	if (inst.opcode == 0x33)// R Instructions
//	{
//		switch (inst.funct3)
//		{
//		case 0:
//			if (inst.funct7 == 32)
//				regs[inst.rd] = regs[inst.rs1] - regs[inst.rs2];
//			else
//				regs[inst.rd] = regs[inst.rs1] + regs[inst.rs2];
//			break;
//		default:
//			cout << "\tUnknown R Instruction \n";
//		}
//	}
//	else
//	{
//		cout << "\tUnknown Instruction \n";
//	}
//
//}

void main()
{
	for (int i = 0; i < 8 * 1024; i++)
		memory[i] = 'n';

	ifstream label_stream;
	string label_string;
	label_stream.open("test.txt");
	if (label_stream.is_open())
	{
		int label_pc = 0;
		while (!label_stream.eof())
		{
			getline(label_stream, label_string);

			if (label_string[label_string.length() - 1] == ':')
			{
				label temp;
				temp.name = label_string.substr(0, label_string.length() - 1);
				temp.address = label_pc;
				labels.push_back(temp);
			}
			label_pc += 4;
		}
		label_stream.close();
	}
	else
		cout << "file did not open" << endl;

	ifstream inst_set;
	int counter = 0;
	inst_set.open("instruction_set.txt");
	if (inst_set.is_open())
	{
		pc = 0x0;
		while (!inst_set.eof())
		{
			inst_set >> instructions_array[counter].name;
			inst_set >> instructions_array[counter].type;
			inst_set >> instructions_array[counter].func3;
			inst_set >> instructions_array[counter].func7;
			inst_set >> instructions_array[counter].m_case;
			counter++;
		}
	}
	else
		cout << "file did not open" << endl;

	inst_set.close();



	ifstream inFile;
	ofstream outFile;
	instWord W;

	inFile.open("test.txt");
	if (inFile.is_open())
	{
		pc = 0x0;
		while (!inFile.eof())
		{
			getline(inFile, W.instText);

			string temp = parse(W);
			if (W.instText[W.instText.length() - 1] != ':')
				pc += 4;

			if (W.instText[W.instText.length() - 1] != ':')
				simulate(W);

			if (W.function == "beq"&& regs[W.rd] == regs[W.rs1]) {

				int count = 0;
				int co = 0;
				string temp = W.instText;
				string rd = myfunc(temp, 'x', ',');
				temp = myfunc(temp, ',', '\0');
				temp = myfunc(temp, ',', '\0');
				while (true)
				{
					if (labels[count++].name == temp || count >= labels.size())
						break;
				}

				inFile.close();
				inFile.open("test.txt");

				--count;

				while (co <= (labels[count].address / 4)) {
					string rubbish;
					getline(inFile, rubbish);
					co++;
				}

				pc = labels[count].address;
			}

			else if (W.function == "bne"&& regs[W.rd] != regs[W.rs1]) {

				int count = 0;
				int co = 0;
				string temp = W.instText;
				string rd = myfunc(temp, 'x', ',');
				temp = myfunc(temp, ',', '\0');
				while (true)
				{
					if (labels[count++].name == temp || count >= labels.size())
						break;
				}
				inFile.close();
				inFile.open("test.txt");

				--count;

				while (co <= labels[count].address / 4) {
					string rubbish;
					getline(inFile, rubbish);
					co++;
				}

				pc = labels[count].address;
			}

			else if (W.function == "blt"&& regs[W.rd] < regs[W.rs1]) {

				int count = 0;
				int co = 0;
				string temp = W.instText;
				string rd = myfunc(temp, 'x', ',');
				temp = myfunc(temp, ',', '\0');
				while (true)
				{
					if (labels[count++].name == temp || count >= labels.size())
						break;
				}
				inFile.close();
				inFile.open("test.txt");

				--count;

				while (co <= labels[count].address / 4) {
					string rubbish;
					getline(inFile, rubbish);
					co++;
				}

				pc = labels[count].address;
			}

			else if (W.function == "bge"&& regs[W.rd] >= regs[W.rs1]) {

				int count = 0;
				int co = 0;
				string temp = W.instText;
				string rd = myfunc(temp, 'x', ',');
				temp = myfunc(temp, ',', '\0');
				while (true)
				{
					if (labels[count++].name == temp || count >= labels.size())
						break;
				}
				inFile.close();
				inFile.open("test.txt");

				--count;

				while (co <= labels[count].address / 4) {
					string rubbish;
					getline(inFile, rubbish);
					co++;
				}

				pc = labels[count].address;
			}

			else if (W.function == "bltu") {
				int rdu, rs1u; string bin1, bin2;
				bin1 = toBinary(regs[W.rd], 32);
				bin2 = toBinary(regs[W.rs1], 32);
				rdu = convertBinaryToDecimal(stoll(bin1));
				rs1u = convertBinaryToDecimal(stoll(bin2));

				if (rdu < rs1u) {
					int count = 0;
					int co = 0;
					string temp = W.instText;
					string rd = myfunc(temp, 'x', ',');
					temp = myfunc(temp, ',', '\0');
					while (true)
					{
						if (labels[count++].name == temp || count >= labels.size())
							break;
					}
					inFile.close();
					inFile.open("test.txt");

					--count;

					while (co <= labels[count].address / 4) {
						string rubbish;
						getline(inFile, rubbish);
						co++;
					}

					pc = labels[count].address;
				}
			}

			else if (W.function == "bgeu") {
				int rdu, rs1u; string bin1, bin2;
				bin1 = toBinary(regs[W.rd], 32);
				bin2 = toBinary(regs[W.rs1], 32);
				rdu = convertBinaryToDecimal(stoll(bin1));
				rs1u = convertBinaryToDecimal(stoll(bin2));

				if (rdu >= rs1u) {
					int count = 0;
					int co = 0;
					string temp = W.instText;
					string rd = myfunc(temp, 'x', ',');
					temp = myfunc(temp, ',', '\0');
					while (true)
					{
						if (labels[count++].name == temp || count >= labels.size())
							break;
					}
					inFile.close();
					inFile.open("test.txt");

					--count;

					while (co <= labels[count].address / 4) {
						string rubbish;
						getline(inFile, rubbish);
						co++;
					}

					pc = labels[count].address;
				}
			}

			else if (W.function == "jal") {

				int count = 0;
				int co = 0;
				string temp = W.instText;
				string rd = myfunc(temp, 'x', ',');
				temp = myfunc(temp, ',', '\0');

				int jumpto;

				if (IsNumericDigit(temp[0]))
					jumpto = stoi(temp);

				else {
					while (true)
					{
						if (labels[count++].name == temp || count >= labels.size())
							break;
					}

					--count;
					jumpto = labels[count].address;
				}
				inFile.close();
				inFile.open("test.txt");

				while (co <= jumpto / 4) {
					string rubbish;
					getline(inFile, rubbish);
					co++;
				}


				if (W.rd != 0) {
					regs[W.rd] = pc;
				}
				pc = jumpto;
			}

			else if (W.function == "jalr") {

				int count = 0;
				int co = 0;
				string temp = W.instText;
				string rd = myfunc(temp, 'x', ',');
				temp = myfunc(temp, ',', '\0');
				temp = myfunc(temp, ',', '\0');
				int jumpto;

				if (IsNumericDigit(temp[0]))
					jumpto = stoi(temp);

				else {
					while (true)
					{
						if (labels[count++].name == temp || count >= labels.size())
							break;
					}

					--count;
					jumpto = labels[count].address;
				}

				if (W.rd != 0) {

					regs[W.rd] = pc;
				}

				pc = jumpto + regs[W.rs1];
				inFile.close();
				inFile.open("test.txt");

				while (co <= pc / 4) {
					string rubbish;
					getline(inFile, rubbish);
					co++;
				}




			}

		}
		inFile.close();


	}
	else
		emitError("Cannot access input file\n");

	system("pause");
}


int HexToDec(char *hexstr, int size) {

	int length = size;
	const int base = 16;     // Base of Hexadecimal Number
	int decnum = 0;
	int i;

	//// Now Find the length of Hexadecimal Number
	//for (hexstr = hex; *hexstr != '\0'; hexstr++) {
	//	length++;
	//}

	// Now Find Hexadecimal Number
	for (i = 0; *hexstr != '\0' && i < length; i++, hexstr++) {
		// Compare *hexstr with ASCII values
		if (*hexstr >= 48 && *hexstr <= 57) {  // is *hexstr Between 0-9
			decnum += (((int)(*hexstr)) - 48) * pow(base, length - i - 1);
		}
		else if ((*hexstr >= 65 && *hexstr <= 70)) {  // is *hexstr Between A-F
			decnum += (((int)(*hexstr)) - 55) * pow(base, length - i - 1);
		}
		else if (*hexstr >= 97 && *hexstr <= 102) {  // is *hexstr Between a-f
			decnum += (((int)(*hexstr)) - 87) * pow(base, length - i - 1);
		}
		else {
			std::cout << "Invalid Hexadecimal Number \n";

		}
	}
	return decnum;
}

string decimalToHex(int num)
{
	vector<int>i;
	int k = 0;
	char s;
	string hex_num = "";
	while (num != 0)
	{

		i.push_back(num % 16);
		num = num / 16;
		k++;
	}
	for (int index = k - 1; index >= 0; index--)
	{
		switch (i[index])
		{
		case 0:
			s = '0';
			break;
		case 1:
			s = '1';
			break;
		case 2:
			s = '2';
			break;
		case 3:
			s = '3';
			break;
		case 4:
			s = '4';
			break;
		case 5:
			s = '5';
			break;
		case 6:
			s = '6';
			break;
		case 7:
			s = '7';
			break;
		case 8:
			s = '8';
			break;
		case 9:
			s = '9';
			break;
		case 10:
			s = 'A';
			break;
		case 11:
			s = 'B';
			break;
		case 12:
			s = 'C';
			break;
		case 13:
			s = 'D';
			break;
		case 14:
			s = 'E';
			break;
		default:
			s = 'F';
		}
		hex_num += s;
	}
	return hex_num;
}

string BinarytoHex32(string Binary)
{
	string a = Binary.substr(0, 16);
	string b = Binary.substr(16);
	int dec1 = convertBinaryToDecimal(stoll(a));
	a = decimalToHex(dec1);
	int dec2 = convertBinaryToDecimal(stoll(b));
	b = decimalToHex(dec2);
	return a + b;
}

string add_binary(long long a, int b)
{
	int sum[20];
	int i = 0;
	int r = 0;

	while (a != 0 || b != 0)
	{
		sum[i++] = (a % 10 + b % 10 + r) % 2;
		r = (a % 10 + b % 10 + r) / 2;
		a = a / 10;
		b = b / 10;
	}
	if (r != 0)
		sum[i++] = r;
	--i;
	string final1;
	while (i >= 0)
		final1 += char(sum[i--] + 48);
	return final1;
}

string toBinary(int n, int n_bits)
{
	// array to store binary number
	int *binaryNum = new int[n_bits];

	for (int y = 0; y < n_bits; y++)
		binaryNum[y] = 0;


	string BinaryString = "";
	// counter for binary array
	int i = 0;
	if (n == 0)
	{
		string temp = "";
		for (int i = 0; i < n_bits; i++)
			temp += "0";
		return temp;
	}

	bool negative = false;
	if (n < 0)
	{
		negative = true;
		n *= -1;
	}

	while (n > 0) {

		// storing remainder in binary array
		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}

	for (int j = n_bits - 1; j >= 0; j--)
	{
		if (binaryNum[j] == 0)
			BinaryString += "0";
		else
			BinaryString += "1";
	}
	if (n_bits > 16)
	{
		bool carry;
		string ones = BinaryString;
		string first_half = BinaryString.substr(0, (n_bits / 2));
		string second_half = BinaryString.substr((n_bits / 2));
		for (int i = 0; i < ones.length(); i++)
		{
			if (second_half[i] == '0')
			{
				carry = false;
				break;
			}
			carry = true;
		}
		second_half = add_binary(stoll(second_half), 1);
		second_half = second_half.substr(0, (n_bits / 2));
		if (carry)
		{
			first_half = add_binary(stoll(first_half), 1);
			first_half = first_half.substr(0, (n_bits / 2));

		}
		ones = first_half + second_half;

	}

	//if negative
	if (negative)
	{
		char c;
		int b;
		string s = BinaryString;
		for (int j = 0; j < n_bits; j++)
		{
			if (s[j] == '0')
				s[j] = '1';
			else
				s[j] = '0';
		}
		string final_value = "";
		if (n_bits > 16)
		{
			bool carry;
			int half = (n_bits / 2);
			string first_half = s.substr(0, half);
			string second_half = s.substr(half);
			for (int i = 0; i < s.length(); i++)
			{
				if (second_half[i] == '0')
				{
					carry = false;
					break;
				}
				carry = true;
			}
			second_half = add_binary(stoll(second_half), 1);
			if (carry)
			{
				first_half = add_binary(stoll(first_half), 1);
				first_half = first_half.substr(0, half);

			}
			string temp2 = "";

			if (n_bits % 2 == 1)
				half++;

			//		for (int i = 0; i < (n_bits/2 - first_half.length()); i++)
			//			temp2 += "1";

			temp2 += first_half;

			for (int i = 0; i < (half - second_half.length()); i++)
				temp2 += "0";

			temp2 += second_half;
			final_value = temp2;

		}
		else
		{
			s = add_binary(stoll(s), 1);

			for (int i = 0; i < (n_bits - s.length()); i++)
				final_value += "1";
			final_value += s;
		}
		return final_value;
	}

	return BinaryString;
}

string myfunc(string &kamel, char begin, char end)
{
	kamel = kamel.substr(1, kamel.length() - 1);
	int x_position = kamel.find(begin);
	int counter = x_position;
	char c = kamel[++counter];
	string a = "";
	while (c != end && counter < kamel.length())
	{
		a += c;
		counter++;
		c = kamel[counter];
	}
	kamel = kamel.substr(x_position + 1);
	return a;
}

void changeMachine(instWord & w)
{
	if (w.instText[w.instText.length() - 1] != ':')
	{
		if (w.mcase == 1)
		{
			string temp = "";
			temp += w.funct7;
			temp += toBinary(w.rs2, 5);
			temp += toBinary(w.rs1, 5);
			temp += w.funct3;
			temp += toBinary(w.rd, 5);
			temp += toBinary(w.opcode, 7);
			w.binarymachinecode = temp;
			w.hexmachinecode = BinarytoHex32(temp);
			MachineCodeVector.push_back(w.hexmachinecode);
		}
		else if (w.mcase == 2)
		{
			string temp = "";
			temp += toBinary(w.rs2, 12);
			temp += toBinary(w.rs1, 5);
			temp += w.funct3;
			temp += toBinary(w.rd, 5);
			temp += toBinary(w.opcode, 7);
			w.binarymachinecode = temp;
			w.hexmachinecode = BinarytoHex32(temp);
			MachineCodeVector.push_back(w.hexmachinecode);
		}
		else if (w.mcase == 3)
		{
			string immediate;
			string temp = "";
			immediate = toBinary(w.rs1, 12);
			string imm1 = immediate.substr(0, 7);
			string imm2 = immediate.substr(7, 4);
			temp += imm1;
			temp += toBinary(w.rd, 5);
			temp += toBinary(w.rs2, 5);
			temp += w.funct3;
			temp += imm2;
			temp += toBinary(w.opcode, 7);
			w.binarymachinecode = temp;
			w.hexmachinecode = BinarytoHex32(temp);
			MachineCodeVector.push_back(w.hexmachinecode);
		}
		else if (w.mcase == 4)
		{
			string temp = "";
			string immediate = toBinary(w.rs1, 32);
			immediate = immediate.substr(0, 20);
			temp += immediate;
			temp += toBinary(w.rd, 5);
			temp += toBinary(w.opcode, 7);
			w.binarymachinecode = temp;
			w.hexmachinecode = BinarytoHex32(temp);
			MachineCodeVector.push_back(w.hexmachinecode);
		}
		else if (w.mcase == 5)
		{
			string temp = "";
			string immediate = toBinary(w.rs1, 21);
			//immediate = immediate.substr(11);
			temp += immediate[0];//msb
			temp += immediate.substr(10, 10);//10-1
			temp += immediate[9];//11
			temp += immediate.substr(1, 8);//19-12
			temp += toBinary(w.rd, 5);
			temp += toBinary(w.opcode, 7);
			w.binarymachinecode = temp;
			w.hexmachinecode = BinarytoHex32(temp);
			MachineCodeVector.push_back(w.hexmachinecode);
		}
		else
		{
			string temp = "";
			string immediate = toBinary(w.rs2, 13);
			temp += immediate[0];//lsb
			temp += immediate.substr(2, 6);//10-5
			temp += toBinary(w.rs1, 5);
			temp += toBinary(w.rd, 5);
			temp += w.funct3;
			temp += immediate.substr(8, 4);//4-1
			temp += immediate[1];
			temp += toBinary(w.opcode, 7);
			w.binarymachinecode = temp;
			w.hexmachinecode = BinarytoHex32(temp);
			MachineCodeVector.push_back(w.hexmachinecode);
		}
	}

}

string parse(instWord &w)

{
	int comment = w.instText.find('#');
	int firstLetter = comment - 1;
	if (comment >= 0)
	{
		while (true)
		{
			if (w.instText[firstLetter] != '\t' && w.instText[firstLetter] != ' ')
				break;
			firstLetter--;
		}
		w.instText = w.instText.substr(0, firstLetter + 1);
	}
	if (w.instText == "ECALL")
	{
		w.opcode = 0x73;

		w.function = "ECALL";
		return " ";
	}
	//if not label
	if (w.instText[w.instText.length() - 1] != ':')
	{

		//getting instruction name for opcode
		int i = 0; string instruction = "";
		while (w.instText[i] != '\t')
		{
			instruction += w.instText[i];
			i++;
		}
		string inst_name = instruction;
		//get type of instruction
		bool flag = false;
		i = 0;
		w.function = inst_name;
		//get instruction type, funcs according to name
		while (!flag)
		{
			if (instruction == instructions_array[i].name)
			{
				flag = true;
				instruction = instructions_array[i].type;
				w.funct3 = instructions_array[i].func3;
				w.funct7 = instructions_array[i].func7;
				w.mcase = instructions_array[i].m_case;
			}
			i++;
		}
		w.type = instruction;
		if (instruction == "R")
		{
			w.opcode = 0x33;
			string temp = w.instText;
			string rd = myfunc(temp, 'x', ',');
			w.rd = stoi(rd);
			string rs1 = myfunc(temp, 'x', ',');
			w.rs1 = stoi(rs1);
			string rs2 = myfunc(temp, 'x', '\0');
			w.rs2 = stoi(rs2);
		}
		else if (instruction == "I")
		{
			w.opcode = 0x13;
			string temp = w.instText;
			int bracket = temp.find('(');
			if (bracket == -1)
			{
				string rd = myfunc(temp, 'x', ',');
				w.rd = stoi(rd);
				string rs1 = myfunc(temp, 'x', ',');
				w.rs1 = stoi(rs1);
				string rs2 = myfunc(temp, ',', '\0');

				if (w.function == "jalr") {
					w.opcode = 0x67;
					string temp = w.instText;
					string rd = myfunc(temp, 'x', ',');
					temp = myfunc(temp, ',', '\0');
					while (temp[temp.length() - 1] == '\t' || temp[temp.length() - 1] == ' ')
					{
						temp = temp.substr(0, temp.length() - 1);

					}
					int count = 0;

					while (true)
					{
						if (labels[count++].name == temp || count >= labels.size())
							break;
					}

					w.rs2 = (labels[--count].address - pc);
				}

				int k = rs2.find('x');
				if (k >= 0)
				{
					rs2 = rs2.substr(rs2.find('x') + 1);
					int stringLength = rs2.length();
					char* hexaString = new char[stringLength];
					for (int i = 0; i < rs2.length(); i++)
					{
						hexaString[i] = rs2[i];
					}
					w.rs2 = HexToDec(hexaString, stringLength);
				}
				else
					w.rs2 = stoi(rs2);
			}
			else
			{
				string rd = myfunc(temp, 'x', ',');
				w.rd = stoi(rd);
				string rs1 = myfunc(temp, ',', '(');
				w.rs1 = stoi(rs1);
				string rs2 = myfunc(temp, 'x', ')');
				int k = rs2.find('x');
				if (k >= 0)
				{
					rs2 = rs2.substr(rs2.find('x') + 1);
					int stringLength = rs2.length();
					char* hexaString = new char[stringLength];
					for (int i = 0; i < rs2.length(); i++)
					{
						hexaString[i] = rs2[i];
					}
					w.rs2 = HexToDec(hexaString, stringLength);
				}
				else
					w.rs2 = stoi(rs2);
			}
		}
		else if (instruction == "U")
		{
			w.opcode = 0x37;
			string temp = w.instText;
			string rd = myfunc(temp, 'x', ',');
			w.rd = stoi(rd);
			string rs1 = myfunc(temp, ',', '\0');
			w.rs1 = stoi(rs1);

		}

		else if (instruction == "UJ")
		{
			w.opcode = 0x6F;
			string temp = w.instText;
			string rd = myfunc(temp, 'x', ',');
			temp = myfunc(temp, ',', '\0');
			w.rd = stoi(rd);
			while (temp[temp.length() - 1] == '\t' || temp[temp.length() - 1] == ' ')
			{
				temp = temp.substr(0, temp.length() - 1);

			}
			int count = 0;

			while (true)
			{
				if (labels[count++].name == temp || count >= labels.size())
					break;
			}

			//w.rs1 = ((labels[--count].address-pc)/4-1)*4+(pc/4)+2;
			//w.rs1 = (pc+labels[--count].address)/4+4;
			//w.rs1 = (labels[--count].address-pc) / 4;
			//int labelInstruction = labels[--count].address / 4;
			//int pcInstruction = pc / 4;
			//pc += 4;
			w.rs1 = (labels[--count].address - pc);
		}

		else if (instruction == "S") {
			w.opcode = 0x23;
			string temp = w.instText;
			string rd = myfunc(temp, 'x', ',');
			w.rd = stoi(rd);
			string rs1 = myfunc(temp, ',', '(');
			w.rs1 = stoi(rs1);
			string rs2 = myfunc(temp, 'x', ')');
			w.rs2 = stoi(rs2);
		}
		else if (instruction == "SB") {
			w.opcode = 0x63;
			string temp = w.instText;
			string rd = myfunc(temp, 'x', ',');
			w.rd = stoi(rd);
			string rs1 = myfunc(temp, 'x', ',');
			w.rs1 = stoi(rs1);
			int count = 0;
			temp = myfunc(temp, ',', '\0');
			while (temp[temp.length() - 1] == '\t' || temp[temp.length() - 1] == ' ')
			{
				temp = temp.substr(0, temp.length() - 1);

			}
			while (true)
			{
				if (labels[count++].name == temp || count >= labels.size())
					break;
			}
			//w.rs2 = (labels[--count].address-pc);
			//int labelInstruction = labels[--count].address / 4;
			//int pcInstruction = pc / 4;
			//pcInstruction++;
			//pc += 4;
			w.rs2 = (labels[--count].address - pc);
		}
		return inst_name;
	}
	else
		return " ";
}

int convertBinaryToDecimal(long long n)
{
	int decimalNumber = 0, i = 0, remainder;
	while (n != 0)
	{
		remainder = n % 10;
		n /= 10;
		decimalNumber += remainder * pow(2, i);
		++i;
	}
	return decimalNumber;
}

void simulate(instWord& w) {
	if (w.function == "add"&& w.rd != 0)
		regs[w.rd] = regs[w.rs1] + regs[w.rs2];

	else if (w.function == "sub"&& w.rd != 0)
		regs[w.rd] = regs[w.rs1] - regs[w.rs2];

	else if (w.function == "sll"&& w.rd != 0)
		regs[w.rd] = regs[w.rs1] * pow(2, regs[w.rs2]);

	else if (w.function == "srl"&& w.rd != 0) {
		if (w.rs1 < 0)
			w.rs1 *= -1;
		regs[w.rd] = regs[w.rs1] / pow(2, regs[w.rs2]);
	}

	else if (w.function == "sra"&& w.rd != 0)
		regs[w.rd] = regs[w.rs1] / pow(2, regs[w.rs2]);

	else if (w.function == "and"&& w.rd != 0)
	{
		string bin1 = toBinary(regs[w.rs1], 32), bin2 = toBinary(regs[w.rs2], 32), value = "";
		for (int i = 0; i < 32; i++) {
			if (bin1[i] == '1'&&bin2[i] == '1')
				value += '1';
			else
				value += '0';
		}
		long long val = stoll(value);
		regs[w.rd] = convertBinaryToDecimal(val);
	}

	else if (w.function == "or"&& w.rd != 0)
	{
		string bin1 = toBinary(regs[w.rs1], 32), bin2 = toBinary(regs[w.rs2], 32), value = "";
		for (int i = 0; i < 32; i++) {
			if (bin1[i] == '1' || bin2[i] == '1')
				value += '1';
			else
				value += '0';
		}
		long long val = stoll(value);
		regs[w.rd] = convertBinaryToDecimal(val);
	}

	else if (w.function == "xor"&& w.rd != 0)
	{
		string bin1 = toBinary(regs[w.rs1], 32), bin2 = toBinary(regs[w.rs2], 32), value = "";
		for (int i = 0; i < 32; i++) {
			if (bin1[i] == bin2[i])
				value += '0';
			else
				value += '1';
		}
		long long val = stoll(value);
		regs[w.rd] = convertBinaryToDecimal(val);
	}

	else if (w.function == "andi"&& w.rd != 0)
	{
		string bin1 = toBinary(regs[w.rs1], 32), bin2 = toBinary(w.rs2, 32), value = "";
		for (int i = 0; i < 32; i++) {
			if (bin1[i] == '1'&&bin2[i] == '1')
				value += '1';
			else
				value += '0';
		}
		long long val = stoll(value);
		regs[w.rd] = convertBinaryToDecimal(val);
	}

	else if (w.function == "ori"&& w.rd != 0)
	{
		string bin1 = toBinary(regs[w.rs1], 32), bin2 = toBinary(w.rs2, 32), value = "";
		for (int i = 0; i < 32; i++) {
			if (bin1[i] == '1' || bin2[i] == '1')
				value += '1';
			else
				value += '0';
		}
		long long val = stoll(value);
		regs[w.rd] = convertBinaryToDecimal(val);
	}

	else if (w.function == "xori"&& w.rd != 0)
	{
		string bin1 = toBinary(regs[w.rs1], 32), bin2 = toBinary(w.rs2, 32), value = "";
		for (int i = 0; i < 32; i++) {
			if (bin1[i] == bin2[i])
				value += '0';
			else
				value += '1';
		}
		long long val = stoll(value);
		regs[w.rd] = convertBinaryToDecimal(val);
	}

	else if (w.function == "addi"&& w.rd != 0)
		regs[w.rd] = regs[w.rs1] + w.rs2;

	else if (w.function == "slt"&& w.rd != 0)
		if (regs[w.rs1] < regs[w.rs2])
			regs[w.rd] = 1;
		else
			regs[w.rd] = 0;

	else if (w.function == "slti"&& w.rd != 0)
		if (regs[w.rs1] < w.rs2)
			regs[w.rd] = 1;
		else
			regs[w.rd] = 0;

	else if (w.function == "sltu"&& w.rd != 0) {
		string bin1 = toBinary(regs[w.rs1], 32), bin2 = toBinary(regs[w.rs2], 32), value = "";

		long long val = stoll(bin1.substr(0, 16));
		long long val15 = stoll(bin1.substr(16));
		int valint;
		valint = convertBinaryToDecimal(val);//rs1
		valint *= pow(2, 16);
		valint += convertBinaryToDecimal(val15);

		long long val2 = stoll(bin2.substr(0, 16));
		long long val25 = stoll(bin2.substr(16));
		int valint2;
		valint2 = convertBinaryToDecimal(val2);//rs2
		valint2 *= pow(2, 16);
		valint2 += convertBinaryToDecimal(val25);

		if (valint < valint2)
			regs[w.rd] = 1;
		else
			regs[w.rd] = 0;
	}

	else if (w.function == "sltiu"&& w.rd != 0) {
		string bin1 = toBinary(regs[w.rs1], 32), bin2 = toBinary(w.rs2, 32), value = "";

		long long val = stoll(bin1.substr(0, 16));
		long long val15 = stoll(bin1.substr(16));
		int valint;
		valint = convertBinaryToDecimal(val);//rs1
		valint *= pow(2, 16);
		valint += convertBinaryToDecimal(val15);

		long long val2 = stoll(bin2.substr(0, 16));
		long long val25 = stoll(bin2.substr(16));
		int valint2;
		valint2 = convertBinaryToDecimal(val2);//rs2
		valint2 *= pow(2, 16);
		valint2 += convertBinaryToDecimal(val25);

		if (valint < valint2)
			regs[w.rd] = 1;
		else
			regs[w.rd] = 0;
	}

	else if (w.function == "slli"&& w.rd != 0)
		regs[w.rd] = regs[w.rs1] * pow(2, w.rs2);

	else if (w.function == "srli"&& w.rd != 0) {
		if (w.rs1 < 0)
			w.rs1 *= -1;
		regs[w.rd] = regs[w.rs1] / pow(2, w.rs2);
	}

	else if (w.function == "srai"&& w.rd != 0)
		regs[w.rd] = regs[w.rs1] / pow(2, w.rs2);

	else if (w.function == "lui"&& w.rd != 0)
	{
		string bin1 = toBinary(regs[w.rs1], 32);
		bin1 = bin1.substr(0, 20);

		for (int i = 0; i < 12; i++)
			bin1 += "0";

		long long val = stoll(bin1);
		regs[w.rd] = convertBinaryToDecimal(val);
	}
	// change in parse, auipc takes label address in rs2
	else if (w.function == "auipc"&& w.rd != 0)
	{
		string bin1 = toBinary(regs[w.rs1], 32);
		bin1 = bin1.substr(0, 20);

		for (int i = 0; i < 12; i++)
			bin1 += "0";

		int dec = convertBinaryToDecimal(stoll(bin1.substr(0, 16)));
		dec *= pow(2, 16);
		dec += convertBinaryToDecimal(stoll(bin1.substr(16)));
		regs[w.rd] = dec + pc;
	}

	else if (w.function == "sb")
	{
		string bin1 = toBinary(regs[w.rd], 32);
		bin1 = bin1.substr(23);

		int dec = regs[w.rs2] + w.rs1;
		int num = convertBinaryToDecimal(stoll(bin1));
		memory[dec] = char(num);

	}

	else if (w.function == "sw")
	{
		string bin1 = toBinary(regs[w.rd], 32);
		int dec = regs[w.rs2] + w.rs1;

		for (int i = 0; i < 4; i++) {
			string temp = "";
			temp += bin1.substr(0, 8);
			bin1 = bin1.substr(8);
			int num = convertBinaryToDecimal(stoll(temp));
			memory[dec + i] = char(num);
		}
	}

	else if (w.function == "sh")
	{
		string bin1 = toBinary(regs[w.rd], 32);
		int dec = regs[w.rs2] + w.rs1;

		for (int i = 0; i < 2; i++) {
			string temp = "";
			temp += bin1.substr(0, 8);
			bin1 = bin1.substr(8);
			int num = convertBinaryToDecimal(stoll(temp));
			memory[dec + i] = char(num);
		}

	}

	else if (w.function == "lh"&& w.rd != 0)
	{
		string bin1 = "";

		int dec = w.rs1 + regs[w.rs2];

		for (int i = 0; i < 2; i++) {
			char tempo = memory[dec + i];
			bin1 += toBinary(int(tempo), 8);
		}

		string temp = "";

		if (bin1[0] == '1')
		{

			for (int o = 0; o < 16; o++)
				temp += '1';
		}

		else
		{
			for (int o = 0; o < 16; o++)
				temp += '0';
		}
		temp += bin1;

		regs[w.rd] = convertBinaryToDecimal(stoll(temp));
	}

	else if (w.function == "lhu"&& w.rd != 0)
	{
		string bin1 = "";

		int dec = w.rs1 + regs[w.rs2];

		for (int i = 0; i < 2; i++) {
			char tempo = memory[dec + i];
			bin1 += toBinary(int(tempo), 8);
		}

		string temp = "";

		for (int o = 0; o < 16; o++)
			temp += '0';

		temp += bin1;

		regs[w.rd] = convertBinaryToDecimal(stoll(temp));
	}

	else if (w.function == "lw"&& w.rd != 0)
	{
		string bin1 = "";

		int dec = w.rs1 + regs[w.rs2];

		for (int i = 0; i < 4; i++) {
			char tempo = memory[dec + i];
			bin1 += toBinary(int(tempo), 8);
		}


		regs[w.rd] = convertBinaryToDecimal(stoll(bin1));
	}

	else if (w.function == "lb"&& w.rd != 0)
	{
		string bin1 = "";

		int dec = w.rs1 + regs[w.rs2];

		char tempo = memory[dec];
		bin1 += toBinary(int(tempo), 8);


		string temp = "";

		if (bin1[0] == '1')
		{

			for (int o = 0; o < 24; o++)
				temp += '1';
		}

		else
		{
			for (int o = 0; o < 24; o++)
				temp += '0';
		}
		temp += bin1;

		regs[w.rd] = convertBinaryToDecimal(stoll(temp));
	}

	else if (w.function == "lbu"&& w.rd != 0)
	{
		string bin1 = "";

		int dec = w.rs1 + regs[w.rs2];

		char tempo = memory[dec];
		bin1 += toBinary(int(tempo), 8);


		string temp = "";


		for (int o = 0; o < 24; o++)
			temp += '0';

		temp += bin1;
		long long val1 = stoll(temp.substr(0, 16));
		long long val2 = stoll(temp.substr(16));
		int value = convertBinaryToDecimal(val1);
		value *= pow(2, 16);
		value += convertBinaryToDecimal(val2);
		regs[w.rd] = value;

	}

	else if (w.instText == "ECALL") {
		if (regs[10] == 1)
			cout << regs[11];

		else if (regs[10] == 4) {
			string bin1 = "";
			int dec = regs[11];

			for (int i = 0; memory[dec + i] != '\0'; i++)
				bin1 += memory[dec + i];


			cout << bin1;
		}

		else if (regs[10] == 5) {
			cin >> regs[11];
		}

		else if (regs[10] == 10)
			system("pause");

	}
}
bool IsNumericDigit(char C)
{
	if (C >= '0' && C <= '9') return true;
	return false;
}