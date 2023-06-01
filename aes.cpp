//Hoang Xuan Quy                                  -git\huyendieu909-
#ifndef BASE_64_HPP
#define BASE_64_HPP

#include <algorithm>
#include <string>

namespace base64 {

inline std::string get_base64_chars() {
    static std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "0123456789+/";
    return base64_chars;
}

// inline
 std::string to_base64(std::string const &data) {
  int counter = 0;
  uint32_t bit_stream = 0;
  const std::string base64_chars = get_base64_chars();
  std::string encoded;
  int offset = 0;
  for (unsigned char c : data) {
    auto num_val = static_cast<unsigned int>(c);
    offset = 16 - counter % 3 * 8;
    bit_stream += num_val << offset;
    if (offset == 16) {
      encoded += base64_chars.at(bit_stream >> 18 & 0x3f);
    }
    if (offset == 8) {
      encoded += base64_chars.at(bit_stream >> 12 & 0x3f);
    }
    if (offset == 0 && counter != 3) {
      encoded += base64_chars.at(bit_stream >> 6 & 0x3f);
      encoded += base64_chars.at(bit_stream & 0x3f);
      bit_stream = 0;
    }
    counter++;
  }
  if (offset == 16) {
    encoded += base64_chars.at(bit_stream >> 12 & 0x3f);
    encoded += "==";
  }
  if (offset == 8) {
    encoded += base64_chars.at(bit_stream >> 6 & 0x3f);
    encoded += '=';
  }
  return encoded;
}

inline std::string from_base64(std::string const &data) {
  int counter = 0;
  uint32_t bit_stream = 0;
  std::string decoded;
  int offset = 0;
  const std::string base64_chars = get_base64_chars();
  for (unsigned char c : data) {
    auto num_val = base64_chars.find(c);
    if (num_val != std::string::npos) {
      offset = 18 - counter % 4 * 6;
      bit_stream += num_val << offset;
      if (offset == 12) {
        decoded += static_cast<char>(bit_stream >> 16 & 0xff);
      }
      if (offset == 6) {
        decoded += static_cast<char>(bit_stream >> 8 & 0xff);
      }
      if (offset == 0 && counter != 4) {
        decoded += static_cast<char>(bit_stream & 0xff);
        bit_stream = 0;
      }
    } else if (c != '=') {
      return std::string();
    }
    counter++;
  }
  return decoded;
}

}

#endif // BASE_64_HPP
using namespace base64;
#include <bits/stdc++.h>
#include "test2.h"
using namespace std;
#define xd cout << "\n"
vector<int> state; // ma trận trạng thái
vector<unsigned char> in; // ma trận đầu vào
vector<unsigned char> out; // ma trận đầu ra
vector<unsigned char> Key; // ma trận Key được sắp xếp lại theo đúng ma trận
vector<unsigned char> RoundKey; // lưu trữ các vòng khóa
int Nb = 4; // số cột của block code, do mã hóa 128 bit nên ma trận 4x4, cần có thể nâng cấp lên 192 hoặc 256 bit sau
int Nr = 10; // số vòng
int Nk = 4; // số cột của khóa gốc
int sz = 0;
char key[100];
string str; // chuỗi cần xử lý
string path; // path đến file
fstream f64; // luồng ghi mã hóa vào ảnh
fstream if64; // luồng ghi thông điệp ra file để sử dụng sau

void InputPath(){
    if (f64.is_open()) f64.close();
    if (if64.is_open()) if64.close();
    f64.open("in.txt",ios::in|ios::out|ios::app);
    if64.open("o64.txt",ios::out|ios::trunc);
}

void InputStr(){
    str = "";
    fflush(stdin);
    getline(cin,str);
}

void InputKey(){
    do {
        cout << "Nhap key: ";
        fflush(stdin);
        gets(key);
    } while (strlen(key) > 16);
    if (strlen(key) < 16) {
        for (int i=strlen(key);i<16;i++) key[i] = 0; 
    }
    int k = 0; // con trỏ key nhập vào
    for (int i=0;i<Nk;i++){
        Key[i] = key[k++];
        Key[i+4] = key[k++];
        Key[i+8] = key[k++];
        Key[i+12] = key[k++];
    }
}

void KeyExpansion(){
    unsigned char temp[4],k;
    for (int i=0 ; i < Nk ; i++) {
        RoundKey[i*4+0] = Key[i*4+0];
        RoundKey[i*4+1] = Key[i*4+1];
        RoundKey[i*4+2] = Key[i*4+2];
        RoundKey[i*4+3] = Key[i*4+3];
    }
    int i = 4;
    while (i < (Nb * (Nr+1))){
        for (int j=0;j<4;j++){
            temp[j] = RoundKey[(i-1)*4+j];
        }
        if (i%Nk==0){
            // rotword
            k = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = k;
            // subword
            temp[0] = getSBoxValue(temp[0]);
            temp[1] = getSBoxValue(temp[1]);
            temp[2] = getSBoxValue(temp[2]);
            temp[3] = getSBoxValue(temp[3]);
            // rcon
            temp[0] = temp[0] ^ Rcon[i/Nk];
        }
        RoundKey[i*4+0] = RoundKey[(i-Nk)*4+0] ^ temp[0];
        RoundKey[i*4+1] = RoundKey[(i-Nk)*4+1] ^ temp[1];
        RoundKey[i*4+2] = RoundKey[(i-Nk)*4+2] ^ temp[2];
        RoundKey[i*4+3] = RoundKey[(i-Nk)*4+3] ^ temp[3];
        i++;
    }
}

void MakeKey(){
    InputKey();
    KeyExpansion();
}

void AddRoundKey(int round) {
   int i,j;
   for (int i=0;i<16;i++){
        state[i] ^= RoundKey[16*round+i];
   }
}

// ========================area for subByte============== //
void SubBytes() {
    for (int i = 0; i < 16; i++)
        state[i] = getSBoxValue(state[i]);
}

void invSubBytes() {
    for (int i = 0; i < 16; i++)
        state[i] = getInvSBoxValue(state[i]);
}
// ====================================================== //

// ================area for shiftRow===================//
void shiftRow(int nbr)
{
    int i, j;
    unsigned char tmp;
    /* each iteration shifts the row to the left by 1 */
    for (i = 0; i < nbr; i++){
        tmp = state[0+nbr*4];
        for (j = 0; j < 3; j++)
            state[j+nbr*4] = state[(j+1)+nbr*4];
        state[3+nbr*4] = tmp;
    }
}
void ShiftRows()
{
    int i;
    /* iterate over the 4 rows and call shiftRow() with that row */
    for (i = 0; i < 4; i++)
        shiftRow(i);
}
void invShiftRow(int nbr)
{
    int i, j;
    unsigned char tmp;
    /* each iteration shifts the row to the right by 1 */
    for (i = 0; i < nbr; i++){
        tmp = state[3+nbr*4];
        for (j = 3; j > 0; j--)
            state[j+nbr*4] = state[(j-1)+nbr*4];
        state[0+nbr*4] = tmp;
    }
}
void invShiftRows()
{
    int i;
    /* iterate over the 4 rows and call invShiftRow() with that row */
    for (i = 0; i < 4; i++)
        invShiftRow(i);
}
// ================================================================== //


void mixColumn(unsigned char *column)
{
    unsigned char cpy[4];
    int i;
    for(i = 0; i < 4; i++){
        cpy[i] = column[i];
    }
    column[0] = galois_multiplication(cpy[0],2) ^
                galois_multiplication(cpy[3],1) ^
                galois_multiplication(cpy[2],1) ^
                galois_multiplication(cpy[1],3);

    column[1] = galois_multiplication(cpy[1],2) ^
                galois_multiplication(cpy[0],1) ^
                galois_multiplication(cpy[3],1) ^
                galois_multiplication(cpy[2],3);

    column[2] = galois_multiplication(cpy[2],2) ^
                galois_multiplication(cpy[1],1) ^
                galois_multiplication(cpy[0],1) ^
                galois_multiplication(cpy[3],3);

    column[3] = galois_multiplication(cpy[3],2) ^
                galois_multiplication(cpy[2],1) ^
                galois_multiplication(cpy[1],1) ^
                galois_multiplication(cpy[0],3);
}
void MixColumns()
{
    int i, j;
    unsigned char column[4];
    /* iterate over the 4 columns */
    for (i = 0; i < 4; i++){
        /* construct one column by iterating over the 4 rows */
        for (j = 0; j < 4; j++){
            column[j] = state[(j*4)+i];
        }
        /* apply the mixColumn on one column */
        mixColumn(column);
        /* put the values back into the state */
        for (j = 0; j < 4; j++){
            state[(j*4)+i] = column[j];
        }
    }
}

void invMixColumn(unsigned char *column)
{
    unsigned char cpy[4];
    int i;
    for(i = 0; i < 4; i++){
        cpy[i] = column[i];
    }
    column[0] = galois_multiplication(cpy[0],14) ^
                galois_multiplication(cpy[3],9) ^
                galois_multiplication(cpy[2],13) ^
                galois_multiplication(cpy[1],11);
    column[1] = galois_multiplication(cpy[1],14) ^
                galois_multiplication(cpy[0],9) ^
                galois_multiplication(cpy[3],13) ^
                galois_multiplication(cpy[2],11);
    column[2] = galois_multiplication(cpy[2],14) ^
                galois_multiplication(cpy[1],9) ^
                galois_multiplication(cpy[0],13) ^
                galois_multiplication(cpy[3],11);
    column[3] = galois_multiplication(cpy[3],14) ^
                galois_multiplication(cpy[2],9) ^
                galois_multiplication(cpy[1],13) ^
                galois_multiplication(cpy[0],11);
}
void invMixColumns()
{
    int i, j;
    unsigned char column[4];
    /* iterate over the 4 columns */
    for (i = 0; i < 4; i++){
        /* construct one column by iterating over the 4 rows */
        for (j = 0; j < 4; j++){
            column[j] = state[(j*4)+i];
        }
        /* apply the invMixColumn on one column */
        invMixColumn(column);
        /* put the values back into the state */
        for (j = 0; j < 4; j++){
            state[(j*4)+i] = column[j];
        }
    }
}

void Cipher() {
    int i,j,round=0;
    //copy to state
    for (i=0 ; i < Nb ; i++) {
        for (j=0 ; j < 4 ; j++) {
	        state[i*4+j] = in[i+4*j];
        }
    }
//dùng dòng dưới để debug khi deploy thì không cần
//cout << "state: "; for (int i =0;i<16;i++) cout << (int) state[i] << " "; cout << "\n";
   // Add the First round key to the state before starting the rounds.
    AddRoundKey(0); 
   // There will be Nr rounds.The first Nr-1 rounds are identical.These Nr-1 rounds are executed in the loop below.
    for (round=1 ; round < Nr ; round++) {
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(round);
    }
   // The last round is given below. The MixColumns function is not here in the last round.
    SubBytes();
    ShiftRows();
    AddRoundKey(Nr);
    //out
    for (i=0 ; i < Nb ; i++) {
        for (j=0 ; j < 4 ; j++) {
	        out[i+4*j]=state[i*4+j];
        }
    }
}

void Plain() {
   int i,j,round=9;
   //Copy the input CypherText to state array.
    for (i=0 ; i < Nb ; i++) {
      for (j=0 ; j < 4 ; j++) {
	 state[i*4+j] = in[i+4*j];
      }
   }
//dùng dòng dưới để debug khi deploy thì không cần
//cout << "state PL: "; for (int i =0;i<16;i++) cout << (int) state[i] << " "; cout << "\n";
   // Add the First round key to the state before starting the rounds.
   AddRoundKey(10); 
   // There will be Nr rounds. The first Nr-1 rounds are identical. These Nr-1 rounds are executed in the loop below.
   for (round=9 ; round >= 1 ; round--) {
      invShiftRows();
      invSubBytes();
      AddRoundKey(round);
      invMixColumns();
   }
   // The last round is given below.
   // The MixColumns function is not here in the last round.
   invShiftRows();
   invSubBytes();
   AddRoundKey(0);
   // Copy the state array to output array.
    for (i=0 ; i < Nb ; i++) {
        for (j=0 ; j < 4 ; j++) {
	        out[i+4*j]=state[i*4+j];
        }
    }
}

int fillBlock () {
   int j=0;
   while (sz < str.size()) {
      if (j >= Nb*4) break;
      in[j++] = str[sz];
      sz++;
   }
   // Pad the block with 0s, if necessary
   if (sz >= str.size()) for ( ; j < Nb*4 ; j++) in[j] = 0;
   return sz;   
}

void Initialize(){
    for (int i=0;i<16;i++) state.push_back(0);
    for (int i=0;i<16;i++) Key.push_back(0);
    for (int i=0;i<176;i++) RoundKey.push_back(0x61);
    for (int i=0;i<16;i++) in.push_back(0);
    for (int i=0;i<16;i++) out.push_back(0);
}

void cypher();
void plain();
void menu();

int main(){
    int choice = 0;
    Initialize();
    menu();
    do{
        cout << "\n___Nhap lua chon: ";
        cin >> choice;
        switch (choice){
            case 1: 
                system("cls");
                cypher();
                cout << "======================Da ma hoa====================\n\n\n\n";
                system("pause");
                menu();
                break;
            case 2:
                system("cls");
                plain();
                cout << "\n=========================ok======================\n\n\n\n";
                system("pause");
                menu();
                break;
            case 3:
                system("cls");
                MakeKey();
                cout << "\n==================Da thay doi key=================\n\n\n\n";
                system("pause");
                menu();
                break;
            case 4:
                system("cls");
                menu();
                break;
            case 5:
                cout << "\nKet thuc chuong trinh\n";
                f64.close();
                system("exit");
                break;
        }
    } while (choice != 5);
}

void menu(){
    cout << "\n=======================================================================\n";
    cout <<   "|------------------------------Menu-----------------------------------|\n";
    cout <<   "|            1. Ma hoa                                                |\n";
    cout <<   "|            2. Giai ma                                               |\n";
    cout <<   "|            3. Thay doi key                                          |\n";
    cout <<   "|            4. Xoa man hinh                                          |\n";
    cout <<   "|            5. Ket thuc chuong trinh                                 |\n";
    cout <<   "| Key: " << key << ""; xd; 
    cout <<   "|---------------------------------------------------------------------|\n";
    cout <<   "=======================================================================\n";
}
void cypher(){
    if (strlen(key)==0) MakeKey();
    if (path.size()<=1) InputPath();
    cout << "Nhap xau ki tu can ma hoa: ";
    InputStr();
    sz = 0;
    while (sz < str.size()) {
        sz = fillBlock();
        Cipher();
        string s = "";
        for (int i=0 ; i < Nb*4 ; i++) s+= out[i];
        s = to_base64(s);
        f64 << s;
    }
    f64 << " ";
    f64 << str.size() << " ";
    int a = scs(str.size());
    f64 << a << " ";
    int b = scs(a);
    f64 << b;
}
void plain(){
    if (strlen(key)==0) MakeKey();
    if (path.size()<=1) InputPath();
    // dịch chuyển con trỏ file để tìm độ dài thông điệp
    int k,a,b,pos=0;
    f64.seekg(-1,ios::end); pos++;
    f64>>b; b++; pos+=b;
    f64.seekg(-pos,ios::end);
    f64>>a; a++; pos+=a;
    f64.seekg(-pos,ios::end);
    f64 >> k; pos++;
    // độ dài thông điệp đã lưu vào biến k, phép toán dưới đây sẽ tìm số khối mã aes cần giải
    int numberOfBlock = k/16;
    if (k%16!=0) numberOfBlock++;
    string s,ss;
    f64.seekg(-(24*numberOfBlock)-pos,ios::end); 
    // cout << f64.tellg() << "\n"; for debug;
    f64 >> ss;
    cout << ss << "\n";
    s = "";
    str = "";
    for (int i=0;i<ss.size();i++){
        s+=ss[i];
        if (i%24==23){
            s = from_base64(s);
            for (int i=0;i<s.size();i++) str += s[i];
            s = "";
        }
    }
    cout << "_____Key: " << key; xd;
    cout << "----------------Thong diep giai ma---------------\n";
    sz = 0;
    while (sz < str.size()) {
        sz = fillBlock();
        Plain();
        for (int i=0;i<16;i++) cout << out[i];
        if64 << "\n";
        for (int i=0;i<16;i++) if64 << out[i];
    }
}