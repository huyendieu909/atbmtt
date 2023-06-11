#include <bits/stdc++.h>
#define ulli unsigned long long int
using namespace std;
ulli p, q;
ulli n, phiN;
ulli e, d, m, c;
ulli tinhMod(ulli a, ulli k, ulli n) {
    ulli res = 1;
    for (ulli i = 0; i < k; i++) {
        res = (res * a) % n;
    }
    return res;
}
ulli gcd(ulli q, ulli r){
	if (r == 0) return q;
	return gcd(r,fmod(q,r));
}

ulli invMod(ulli a, ulli m){
    if (gcd(a,m)==1) {
        a = a % m;
        for (ulli i = 1;i < m;i++){
            if ((a*i) % m == 1) return i;
        }
    }
    return -1;
}
bool isPrime(ulli a){
	if (a < 2) return false;
	for (ulli i = 2;i<=sqrt(a);i++)
		if (a%i==0) return false;
	return true;
}
void input(){
	do{
		q = rand() %100000 +1;
	} while (!isPrime(q));
	do{
		p = rand() %100000 +1;
	} while (!isPrime(p) || p==q);
	
	n = p*q;
	phiN = (p-1)*(q-1);
	do {
		e = rand() %(phiN-2)+2;
	} while (gcd(e,phiN) != 1);
	
	d = invMod(e,phiN);	
}
void nhapThongTin(){
	cout << "Nhap thong diep: ";
	cin >> m;
}
ulli cypher(){
    c = tinhMod(m,e,n);
	return c;
}
ulli plain(){
    m = tinhMod(c,d,n);
    return m;
}

int main(){
	input();
	nhapThongTin();
	cout << "Cypher: " << cypher();
    cout << "\nPlain: " << plain();
    cout << "\nd: " << d;
    cout << "\ne: " << e;
    cout << "\nphiN: " << phiN;
}