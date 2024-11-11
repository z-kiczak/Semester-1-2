#include<bits/stdc++.h>
#include"kol.h"
using namespace std;

#define st first
#define nd second 

vector < pair < interesant*, interesant* > > kolejka; //wskaźniki na koniec (first) i początek (second) i-tej kolejki
int akt_numerek; //aktualny numerek który zostanie przydzielony następnej osobie w urzędzie
int M;

//otiwera m okienek w urzędzie
void otwarcie_urzedu(int m){
    M = m;
    akt_numerek = 0;
    for(int i = 0; i < m; i++) kolejka.push_back({nullptr, nullptr});
}

void czysc(interesant *i){
    i->s1 = nullptr; i->s2 = nullptr; 
}

//łączy wskaźnikami interesanta i oraz interesanta j (obustronnie!)
//zakładamy że oba mają po co najmniej jednym wskaźniku wolnym
void polacz(interesant *i, interesant *j){
    assert(i != nullptr); assert(j != nullptr);
    if(i->s1 == nullptr) i->s1 = j;
    else i->s2 = j;

    if(j->s1 == nullptr) j->s1 = i;
    else j->s2 = i;
}

//odczepia od siebie interesanta i, j (obustronnie!)
//zakładamy że są ze sobą złączone
void rozlacz(interesant *i, interesant *j){
    if(i->s1 == j) i->s1 = nullptr;
    else i->s2 = nullptr;

    if(j->s1 == i) j->s1 = nullptr;
    else j->s2 = nullptr;
}

//zwraca następnego po i interesanta, jeżeli poprzedni to j
//tzn. j sąsiaduje z i, wskazuje ,,kierunek''; 
//uwaga! j może być nullptr'em, wtedy i znajduje się na krańcu kolejki
interesant *nast(interesant *i, interesant *j){
    if(i->s1 == j) return i->s2;
    else return i->s1;
}

//x dochodzi na koniec kolejki k
//kolejne osoby między x i y ustawiają się pomiędzy w tej samej kolejności
//y jest nowym końcem kolejki
void nowe_w_kolejce(interesant *x, interesant *y, int k){
    if(kolejka[k].nd == nullptr) kolejka[k].nd = x;
    else polacz(x, kolejka[k].st);
    kolejka[k].st = y;

    x->k = k; y->k = k;
}

//zwraca wskaźnik do nowego interesanta, stojącego w kolejce k
interesant *nowy_interesant(int k){
    interesant* x = (interesant*)malloc(sizeof(interesant));
    x->num = akt_numerek;
    akt_numerek ++;

    czysc(x); //,,zeruje'' wartości
    nowe_w_kolejce(x, x, k);
    return x;
}

//zwraca numerek interesanta
int numerek(interesant *i){
    return i->num;
}

interesant *obsluz(int k){
    if(kolejka[k].nd == nullptr) return nullptr;

    interesant* x = kolejka[k].nd;
    interesant* y = nast(x, nullptr);
  
    kolejka[k].nd = y;
    if(y == nullptr){ kolejka[k].st = nullptr; }
    else{ rozlacz(x, y); y->k = k; }
    return x;
}

//zwraca wektor kolejnych w kolejce interesantów od a do b
//a stoi w kolejce PRZED b
vector < interesant *> pomoc(interesant *a, interesant *b){
    vector < interesant *> v1, v2;
    v1.push_back(a); v2.push_back(a);
    if(a == b) return v1;

    interesant* a11 = a; interesant* a1 = a->s1;
    interesant* a22 = a; interesant* a2 = a->s2;
    v1.push_back(a1); v2.push_back(a2);
    while( (a1 != b) and (a2 != b) ){
        if(a1 != nullptr){ 
            auto t = a1;
            a1 = nast(a1, a11);
            a11 = t;
        }
         if(a2 != nullptr){ 
            auto t = a2;
            a2 = nast(a2, a22);
            a22 = t;
        }
        v1.push_back(a1); v2.push_back(a2);
    }
    if(a1 == b) return v1;
    else return v2;
}

//usuwa intersantów od a do b gdzie a stoi w kolejce PRZED b; 
//aktualizuje początek i koniec kolejki, jeżeli to potrzebne
void usun(interesant *a, interesant *b){
    vector < interesant *> v = pomoc(a, b);
    interesant *c; interesant *d;

    if(v.size() == 1){
        c = a->s1;
        d = a->s2;
        if( (c == nullptr) and (kolejka[a->k].st == a) )swap(c,d);
        if( (d == nullptr) and (kolejka[a->k].nd == a) )swap(c,d);
    }
    else{
        c = nast(a, v[1]);
        d = nast(b, v[v.size() - 2]);
    }

    if(c == nullptr){
        kolejka[a->k].nd = d;
        if(d != nullptr)d->k = a->k;
    }
    else{ rozlacz(a, c); }
   
    if(d == nullptr){
        kolejka[b->k].st = c;
        if(c != nullptr)c->k = b->k;
    }
    else{ rozlacz(b, d); }

    if( (c != nullptr) and (d != nullptr) ) polacz(c,d);
}

//interesant i, zmienia kolejkę w której stoi na kolejkę k
void zmiana_okienka(interesant *i, int k){
    usun(i,i);
    nowe_w_kolejce(i, i, k);
}

//kolejka k się odwraca
void naczelnik(int k){
    swap(kolejka[k].st, kolejka[k].nd);
}

//okienko k1 się zamyka, a interesanci stojący w kolejce przechodzą do okienka k2
//- w kolejności w jakiej stali
void zamkniecie_okienka(int k1, int k2){ 
    if(kolejka[k1].nd != nullptr)
        nowe_w_kolejce(kolejka[k1].nd, kolejka[k1].st, k2);
    
    kolejka[k1] = {nullptr, nullptr};
}

//interesanci od i1 do i2 przechodzą do okienka specjalnego, gdzie są od razu obsługiwani
//i1 stoi w tej samej kolejce PRZED i2 (chyba że i1 == i2)
//zwaraca wektor interesantów w kolejności obsłużenia
std::vector<interesant *> fast_track(interesant *i1, interesant *i2){
    vector <interesant *> v = pomoc(i1, i2);
    usun(i1, i2);
    return v;
}

//zwraca wektor interesantów którzy jeszcze stali w kolejkach,
//uporządkowanych wg numeru okienka i następnie porządku kolejki
std::vector<interesant *> zamkniecie_urzedu(){
    vector <interesant *> v;
    for(int i = 0; i < M; i++){
        while(!(kolejka[i].nd == nullptr)) v.push_back( obsluz(i) );
    }
    kolejka.clear();
    return v;
}

