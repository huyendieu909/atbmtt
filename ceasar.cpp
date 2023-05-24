#include <bits/stdc++.h>
using namespace std;
void menu(string s, int k){
	cout << "\n====================================\n";
	cout << "\tChuoi ki tu hien tai: " << s << " \t k = " << k << " \n";
    cout << "Lựa chọn công việc:\n";
    cout << "1. Mã hóa chuỗi ceasar\n";
    cout << "2. Giải mã chuỗi ceasar\n";
    cout << "3. Nhap gia tri k va chuoi s\n";
    cout << "4. Ket thuc chuong trinh\n";
}
void nhapThongTin(string &s, int &k){
    cout << "Nhập vào chuỗi ký tự cần thao tác: "; fflush(stdin);
    getline(cin,s); fflush(stdin);
    cout << "Nhap khoa k: ";
    cin >> k;
}
void maHoa(string &s,string &ss, int k){
    ss = "";
    int a;
    for(int i=0;i<s.size();i++){
        if (s[i] != ' ') a = (int)(s[i])+k;
        else a = (int) s[i];
        if (a > 'z') a-=26;
        ss += (char)(a);
    }
    cout << "---------------------------------------------\n";
    cout << "Chuỗi sau khi mã hóa ceasar: "<<ss;
}
void giaiMa(string &s, string &ss, int k){
    s = "";
    int a;
    for(int i=0;i<ss.size();i++){
        if (ss[i] != ' ')a = (int)(ss[i])-k;
        else a = (int) ss[i];
        if (a < 'a' && ss[i] != ' ') a+=26;
        s += (char)(a);
    }
    cout << "\n---------------------------------------------\n";
    cout << "Chuỗi sau khi giải mã hóa ceasar: "<<s;
}
void luaChon(int &a, string &s, string &ss, int k){
    if (a==1){
        maHoa(s,ss,k);
        s = ss;
        a = 5;
    }
    if (a==2){
        giaiMa(ss,s,k);
        s = ss;
        a = 5;
    }
    if (a==3){
    	nhapThongTin(s,k);
    	a = 5;
    }
    if (a==4){
    	cout << "Ket thuc chuong trinh";
    	exit(0);
    }
}
int main(){
    system("chcp 65001");
    string s,ss,sss;
    int a,k;
    nhapThongTin(s,k);
    do {
		menu(s,k);
    	cout << "Nhập lựa chọn: "; 
	    cin >> a;
		luaChon(a,s,ss,k);
	    cout << "\n";
    } while (a!=4 && a!=3 && a!=1 && a!=2);
}