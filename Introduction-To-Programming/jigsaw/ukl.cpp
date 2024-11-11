#include <vector>
#include <cstdio>
#include <algorithm>

using namespace std;

struct klocek{
    vector < char > t [4]; //wektory opisujące klocek w 4 orientacjach
    int szer [4]; //wymiar klocka w danej orientacji na szerokość
    int wys [4]; //na wysokosc - jeśli dany klocek nie jest brany pod uwagę =-1
    int roz, nr; //rozmiar (ilość pól) klocka, jego nr na wejściu
    klocek()
    {
        for(int i = 0; i < 4; i++) { szer[i] = 0; wys[i] = 0; }
        roz = 0;nr = 0;
    }
};  

int n, m, k;
vector < klocek > T;

bool cmp(klocek a, klocek b){
    if(a.roz == b.roz) return a.nr > b.nr;
    return a.roz > b.roz;
}

bool rowne(vector < char > a, vector < char > b){
    if( a.size() != b.size() ) return false;
    for(int i = 0; i < (int)a.size(); i++){
        if(a[i] != b[i]) return false;
    }
    return true;
}

//tworzy klocek, okrojony maksymalnie z każdej strony, liczy jego wymiary na długość i szerokość 
klocek zrob_klocek(vector < char > v, int num){
    char p[11][11]; //tablica pomocnicza
    klocek wyn;
    wyn.nr = num;
    wyn.roz = 0;

    for(int i = 0; i < (int)v.size(); i++){
        if(v[i] != '.') wyn.roz ++;
        p[i / m][i % m] = v[i];
    }
    
    //okrajanie klocka
    int bi = n - 1, ei = 0, bj = m - 1, ej = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(p[i][j] != '.'){
                bi = min(bi, i);
                ei = max(ei, i);
            }
        }
    }
    for(int j = 0; j < m; j++){
        for(int i = 0; i < n; i++){
            if(p[i][j] != '.'){
                bj = min(bj, j);
                ej = max(ej, j);
            }
        }
    }

    //obracanie klocka
    int n2 = ei - bi + 1, m2 = ej - bj + 1;
    
    for(int i = bi; i <= ei; i++)
        for(int j = bj; j <= ej; j++)
            wyn.t[0].push_back(p[i][j]);
    
    for(int j = ej; j >= bj; j--)
        for(int i = bi; i <= ei; i++)
            wyn.t[1].push_back(p[i][j]);
    
    for(int i = ei; i >= bi; i--)
        for(int j = ej; j >= bj; j--)
            wyn.t[2].push_back(p[i][j]);

    for(int j = bj; j <= ej; j++)
        for(int i = ei; i >= bi; i--)
            wyn.t[3].push_back(p[i][j]);

    //eliminacja klocków wystających poza planszę i powtarzających się
    for(int i = 0; i < 4; i++){
        if(i % 2 == 0){ wyn.wys[i] = n2; wyn.szer[i] = m2; }
        else{ wyn.wys[i] = m2; wyn.szer[i] = n2; }

        if((wyn.wys[i] > n) or (wyn.szer[i] > m)) wyn.wys[i] = -1;
        if( (i > 0) and (rowne(wyn.t[i], wyn.t[i - 1]))) wyn.wys[i] = -1;
    }

    return wyn;
}

void wczytaj(){
    int licz = 0;

    scanf("%d %d %d",&n,&m,&k);
    for(int z = 1; z <= k; z++){
        vector < char > v;
        for(int i = 1; i <= n*m; i++){
            char x; scanf(" %c",&x);
            v.push_back(x);
        }
        
        T.push_back( zrob_klocek(v, licz ++) );
    }
    sort(T.begin(), T.end(), cmp); //sortuje klocki malejąco po rozmiarze
}

bool nie_nachodzi(int I, int J, klocek x, int obrot, vector < char > &w, int p){

    for(int i = I; i < I + x.wys[obrot]; i++){
        for(int j = J; j < J + x.szer[obrot]; j++){

            int nr = i * m + j;
            int nr2 = (i - I) * x.szer[obrot] + (j - J);

            if(p == 1){
                if((w[nr] != '.') and (x.t[obrot][nr2] != '.')) return false;
                if(x.t[obrot][nr2] != '.') w[nr] = x.t[obrot][nr2];
            }
            else{
                if(w[nr] == x.t[obrot][nr2]) w[nr] = '.'; //cofanie zmian
            }
        }
    }
    return true;
}

bool rek(vector < klocek > &v, vector < char > &w, int wsk){
    if(wsk == (int)v.size()) return true;

    klocek K = v[wsk];
    for(int obrot = 0; obrot < 4; obrot++){
        if(K.wys[obrot] == -1) continue;

        for(int i = 0; i <= n - K.wys[obrot]; i++){
            for(int j = 0; j <= m - K.szer[obrot]; j++){
                if(nie_nachodzi(i, j, K, obrot, w, 1)){ 
                    if( rek(v, w, wsk + 1) ) return true;
                }
                nie_nachodzi(i, j, K, obrot, w, -1); //przywracanie stanu poprzedniego w
            }
        }
    }
    return false;
}

//jeżeli istnieje poprawne ustawienie, zwraca je; jeżeli nie, zwraca pusty wektor
vector < char > ustaw(vector < klocek > &v){
    vector < char > w;
    for(int i = 1; i <= n * m; i++) w.push_back('.');

    bool odp = rek(v, w, 0);
    if(odp) return w;
    w.clear();
    return w;
}

void wypisz(){
    bool istnieje = false;
    vector < char > rozw;

    for(int mask = 1; mask < (1 << k); mask ++){
        vector < klocek > v;
        int sum = 0;

        for(int p = 0; p < k; p++){
            if(mask & (1 << p)){
                v.push_back(T[p]);
                sum += T[p].roz;
            }
        }
        if(sum == n * m){
            rozw = ustaw(v);
            if(rozw.size() != 0){ 
                istnieje = true; 
                break;
            }
        }
    }
    if(!istnieje) puts("NIE");
    else{
        puts("TAK");
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++)
                printf("%c", rozw[ i * m + j]);
            printf("\n");
        }
        puts(" ");
    }    
}

int main(){
    wczytaj();
    wypisz();
}