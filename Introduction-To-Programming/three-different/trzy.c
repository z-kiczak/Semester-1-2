#include<stdio.h>

#define ll long long
#define max(x,y) ( (x)<(y) ? (y):(x) )
#define min(x,y) ( (x)>(y) ? (y):(x) )

const ll MAXN = 1e18 + 7LL;

typedef struct motel{
    int nr;
    ll dist;
} motel;

int n;
motel t[1000007]; //tablica na wejściu

ll mini, maxi; //przechowuje aktualnie znalezioną najmniejszą i największą odległość

typedef struct cls{
    //numer najbliższego i drugiego najbliższego motelu, o różnych numerach, z lewej - prev - i z prawej - next - strony; jeżeli nie ma takiego motelu - zero
    int prev, prev2, next, next2;
} cls;

cls p [1000007]; 
//dla każdej pozycji w tablicy wejśćiowej, pozycje dwóch najblizszych z lewej i prawej strony moteli o róznych numerach

int pocz [7]; //pozycje w tablicy t trzech moteli o różnych numerach najbardziej na lewo 
int kon [7]; //pozycje w tablicy t trzech moteli o różnych numerach najbardziej na prawo

//aktualizacja mini i maxi, biorąc pod uwagę trójkę na pozycjach 0 <= i,j,k <= n, jeżeli któraś pozycja jest zerem -> nie istnieje
void act(int i, int j, int k){
    //wykluczamy przypadki w których któreś dwa motele mają ten sam numer
    if(t[i].nr == t[j].nr) return;
    if(t[i].nr == t[k].nr) return;
    if(t[j].nr == t[k].nr) return;
    //wykluczamy nieistniejące pozycje
    if( ((i == 0) || (j == 0)) || (k == 0) ) return;

    //upewniamy się ze i,j,k zostały podane w odpowiedniej kolejności, tzn i < j < k
    if( (i > j) | (i > k) ) return;
    if( j > k ) return;
   
    //aktualizujemy wyniki
    mini = min(mini, max(t[j].dist - t[i].dist, t[k].dist - t[j].dist));
    maxi = max(maxi, min(t[j].dist - t[i].dist, t[k].dist - t[j].dist));
    return;
}

int main(){
    scanf("%d",&n);
    for(int i = 1; i <= n; i++){
        scanf("%d %lld",&t[i].nr,&t[i].dist);
    }
    //tablica na wejściu jest posortowana niemalejąco względem dist
    mini = MAXN; maxi = 0LL;
    
    //znajdywanie najdalszej trójki
    //uzupełnianie tablic pocz i kon
    int wsk = 0;
    for(int i = 1; i <= n; i++){
        if( (t[i].nr != t[pocz[wsk]].nr) && (t[i].nr != t[pocz[max(0,wsk - 1)]].nr) ){
            wsk++;
            pocz[wsk] = i; 
        }
        if(wsk == 3) break;
    }
    if(wsk < 3){printf("0 0\n"); return 0;} //nie istnieją trzy różne motele

    wsk = 0;
    for(int i = n; i >= 1; i--){
        if( (t[i].nr != t[kon[wsk]].nr) && (t[i].nr != t[kon[max(wsk - 1, 0)]].nr) ){
            wsk++;
            kon[wsk] = i;
        }
        if(wsk == 3) break;
    }

    //aktualizacja wyniku na podstawie pocz i kon
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= 3; j++){
            for(int k = 1; k <= 3; k++){
                act(pocz[j], i, kon[k]);
            }
        }
    }
  
    //znajdywanie najbliższej trójki
    //uzupełnianie tablicy p - znajdywanie dwóch najbliższych hoteli o różnych numerach:
    //z lewej strony
    int p1 = 0, p2 = 0;
    for(int i = 1; i <= n; i++){
        while( (i < n) && (t[i].nr == t[i + 1].nr) ){
            p[i].prev2 = p1; p[i].prev = p2;
            i++;
        }
        p[i].prev2 = p1; p[i].prev = p2;

        p1 = p2;
        p2 = i;
    }
    
    //z prawej strony
    p1 = 0; p2 = 0;
    for(int i = n; i >= 1; i--){
        while( (i > 1) && (t[i].nr == t[i - 1].nr) ){
            p[i].next2 = p1; p[i].next = p2;
            i--;
        }
        p[i].next2 = p1; p[i].next = p2;

        p1 = p2;
        p2 = i;
    }

    //rozpatrzymy wszystkie optymalne położenia moteli, bo w sytuacji (x, y , x, y, x), na pewno środkowy x nie należałby do optymalnego rozwiązania
    //(gdyż optymalniej byłoby wziąć jednego ze skrajnych x-ów) natomiast w dowolnej innej sytuacji są to trzy różne motele - ,,kandydaci'' do najbliższej trójki
    //łączenie znalezionych ,,najbliższych" moteli z lewej i z prawej w trójki, aktualizacja wyniku
    for(int i = 1; i <= n; i++){
        act(p[i].prev, i, p[i].next);
        act(p[i].prev, i, p[i].next2);
        act(p[i].prev2, i, p[i].next);
        act(p[i].prev2, i, p[i].next2);
    }

    //wypiswanie wyniku
    if(mini == MAXN) mini = 0LL;
    printf("%lld %lld\n",mini,maxi);
    return 0;
}