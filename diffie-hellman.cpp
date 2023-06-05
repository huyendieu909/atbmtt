#include <iostream>
#include <cmath>
using namespace std;

// Hàm tính a mu k mod n
int power(int a, int k, int n) {
    int res = 1;
    a = a % n;
    while (k > 0) {
        if (k & 1) {
            res = (res * a) % n;
        }
        k = k >> 1;
        a = (a * a) % n;
    }
    return res;
}

// Hàm tính khóa công khai
// PrK: private key
int getPublicKey(int q, int a, int PrK) {
    return power(a, PrK, q);
}

// Hàm tính khóa chung
int getSharedKey(int q, int publicKey, int secretKey) {
    return power(publicKey, secretKey, q);
}

int main() {
    int q = 353; // Tham s? q
    int g = 3; // Tham s? g

    // Bên A ch?n s? ng?u nhiên a
    int a = 97; // Khóa bí m?t c?a bên A
    int A = getPublicKey(q, g, a); // Khóa công khai c?a bên A

    // Bên B ch?n s? ng?u nhiên b
    int b = 233; // Khóa bí m?t c?a bên B
    int B = getPublicKey(q, g, b); // Khóa công khai c?a bên B

    // Bên A tính khóa chung
    int sharedKeyA = getSharedKey(q, B, a);

    // Bên B tính khóa chung
    int sharedKeyB = getSharedKey(q, A, b);

    // Ki?m tra tính dúng d?n c?a k?t qu?
    cout << "Shared key A: " << sharedKeyA << endl;
    cout << "Shared key B: " << sharedKeyB << endl;

    return 0;
}