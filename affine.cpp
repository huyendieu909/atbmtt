#include <bits/stdc++.h>
using namespace std;
string str;
int a=-1,b,aInv;
unordered_map<char,int> cTI; // char to int
unordered_map<int,char> iTC; // int to char
string cyp;
string pln;
int gcd(int a, int m){
    if (m == 0) return a;
    return gcd(m,a%m);
}
int invMod(int a, int m){
    if (gcd(a,m)==1) {
        a%=m;
        for (int i = 1;i < m;i++){
            if (a*i % m == 1) return i;
        }
    }
    return -1;
}
void Input(){
    do {
        cout << "Nhap a (0 -> 25, gcd(a,26)=1): ";
        cin >> a;
    } while (gcd(a,26) != 1 || a < 0 || a > 25);
    aInv = invMod(a,26);
    do{
        cout << "Nhap b (0 -> 25): ";
        cin >> b;
    } while (b < 0 || b > 25);
    str = "";
    cout << "Nhap ki tu can thao tac: ";
    fflush(stdin);
    getline(cin,str);
}

string cypher(){
    if (str.size() < 1||a==-1) Input();
    for (int i = 0;i<str.size();i++){
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
            int so = cTI[str[i]];
            so = (a*so+b) % 26;
            cyp += (iTC[so]);
        }
        else cyp+= str[i];
    }
    str = cyp; cyp = "";
    return str;

}
string plain(){
    if (str.size() < 1||a==-1) Input();
    for (int i = 0;i<str.size();i++){
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
            int so = cTI[str[i]];
            if (so-b<0) so+=26;
            so = (aInv *(so - b)) % 26;
            pln += (iTC[so]);
        }
        else pln += str[i];
    }
    str = pln; pln = "";
    return str;
}

void menu(){
    cout << "\n=======================================================================\n";
    cout <<   "|------------------------------Menu-----------------------------------|\n";
    cout <<   "|            1. Ma hoa                                                |\n";
    cout <<   "|            2. Giai ma                                               |\n";
    cout <<   "|            3. Thay doi a, b, chuoi ki tu                            |\n";
    cout <<   "|            4. Xoa man hinh                                          |\n";
    cout <<   "|            5. Ket thuc chuong trinh                                 |\n";
    cout <<   "| Chuoi ki tu hien tai: "<<str<<"\n";
    cout <<   "| a = "<<a<<", b = "<<b<<"\n"; 
    cout <<   "|---------------------------------------------------------------------|\n";
    cout <<   "=======================================================================\n";
}
void Initialize(){
    char c = 'a',cC = 'A';
    for (int i = 0;i<26;i++){
        iTC[i] = cC++;
    }
    c = 'a',cC = 'A';
    for (int i = 0;i<26;i++){
        cTI[c++] = i;
        cTI[cC++] = i;
    }
}
int main(){
    Initialize();
    int choice;
    do {
        menu();
        cout << "___Nhap lua chon: ";
        cin >> choice;
        switch(choice){
            case 1:
                system("cls");
                cypher();
                cout << "Thong diep ma hoa: " << str  << "\n";
                cout << "===============Da ma hoa=============\n\n\n\n";
                system("pause");
                break;
            case 2: 
                system("cls");
                plain();
                cout << "Thong diep giai ma: " <<str << "\n";
                cout << "================Done=================\n\n\n\n";
                system("pause");
                break;
            case 3:
                system("cls"); 
                Input();
                cout << "=============Da thay doi============\n\n\n\n"; 
                break;
            case 4: 
                system("cls");
                break;
            case 5: 
                cout << "Chuong trinh ket thuc.\n";
                system("exit");
        }
    } while (choice != 5);
}