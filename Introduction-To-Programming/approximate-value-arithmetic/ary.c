#include<stdio.h>
#include<math.h>
#include "stdbool.h"
#include"ary.h"

const double dokladnosc = 0.0000000001;

double max(double x, double y){
    if(x - y > 0.0) return x;
    return y;
}
double min(double x, double y){
    if(x - y > 0.0) return y;
    return x;
}

//sprawdza czy a jest przedziałem pustym, reprezentowanym jako <inf, -inf>
bool pusty(wartosc a){
    if( isinf(a.pocz) && isinf(a.kon) ){
        if(a.pocz > 0.0) return true;
    }
    return false;
}

//zwraca prawdę jeśli liczba x jest zerem, w granicy przyjętej dokładności
bool iszero(double x) { return fabs(x) < dokladnosc; }

/* Implicite zakładamy, że wszystkie argumenty typu double są liczbami  */
/* rzeczywistymi, tzn. są różne od HUGE_VAL, -HUGE_VAL i NAN. */

/* wartosc_od_do(x,y,&wynik) ustawia wynik na [x;y]       */
/* warunek poczatkowy: x <= y                             */
wartosc wartosc_od_do(double x, double y){
    return (wartosc){.pocz = x, .kon = y};
}

//robi to samo co wartosc_od_do, ale bez warunku początkowego na x <= y, x i y mogą być w dowolnej relacji (formalność)
wartosc wartosc_od_do2(double x, double y){
    return (wartosc){.pocz = x, .kon = y};
}

/* wartosc_dokladnosc(x, p, &wynik) ustawia w na x +/- p% */
/* warunek początkowy: p > 0                              */
wartosc wartosc_dokladnosc(double x, double p){
    double a = x - (x * (p / 100.0)), b = x + (x * (p / 100.0));
    //min i max, ponieważ p% z x może być ujemne, niekoniecznie dodatnie
    return wartosc_od_do2(  min(a,b), max(a,b) );
}

/* wartosc_dokladna(x, &wynik) ustawia wynik na [x;x]     */
wartosc wartosc_dokladna(double x){
    return wartosc_od_do2(x,x);
}

/* in_wartosc(&w, x) = x \in w */
bool in_wartosc(wartosc w, double x){
    if(pusty(w)) return false;
    
    if(w.pocz <= w.kon){
        if( (x >= w.pocz) && (x <= w.kon) )return true;
        return false;
    }
    //przypadek gdy w.pocz > w.kon, tzn. przedzial = <x.pocz, MAX_VAL> + <-MAX_VAL, x.kon>
    if( (x >= w.pocz) || (x <= w.kon) ) return true;
    return false;
}

/* min_wartosc(&w) = najmniejsza możliwa wartość w,       */
/* lub -HUGE_VAL jeśli brak dolnego ograniczenia.         */
double min_wartosc(wartosc w){
    if(pusty(w)) return NAN;

    if(w.pocz <= w.kon) return w.pocz;
    //jeżeli w.pocz > w.kon, nie ma dolnego ograniczenia
    return -HUGE_VAL;
}

/* max_wartosc(&w) = największa możliwa wartość w,        */
/* lub HUGE_VAL jeśli brak górnego ograniczenia.          */
double max_wartosc(wartosc w){
    if(pusty(w)) return NAN;

    if(w.pocz <= w.kon) return w.kon;
    //jeżeli w.pocz > w.kon, nie ma górnego ograniczenia
    return HUGE_VAL;
}

/* środek przedziału od min_wartosc do max_wartosc,       */
/* lub nan jeśli min i max_wartosc nie są określone.      */
double sr_wartosc(wartosc w){
    if(pusty(w)) return NAN;
    //zgodnie z treścią zadania, jeżeli przedział zawiera inf i -inf zwracamy NAN
    if( isinf(max_wartosc(w)) && isinf(min_wartosc(w)) ) return NAN;

    return (w.pocz + w.kon) / 2.0;
}

/* Operacje arytmetyczne na niedokładnych wartościach.    */
wartosc plus(wartosc a, wartosc b){
    //jeżeli któryś przedział jest pusty, wynikiem będzie przedział pusty
    if( pusty(a) || pusty(b) ) return wartosc_od_do2(HUGE_VAL, -HUGE_VAL);

    if(in_wartosc(a, HUGE_VAL) && in_wartosc(b, -HUGE_VAL)) return wartosc_od_do2(-HUGE_VAL, HUGE_VAL);
    if(in_wartosc(a, -HUGE_VAL) && in_wartosc(b, HUGE_VAL)) return wartosc_od_do2(-HUGE_VAL, HUGE_VAL);
    //wykluczyliśmy możliwość, że jednocześnie a.pocz > a.kon i b.pocz > b.kon

    if( (a.pocz <= a.kon) && (b.pocz <= b.kon) ) return wartosc_od_do2(a.pocz + b.pocz, a.kon + b.kon);

    //został przypadek, w którym dla jednego przedziału pocz > kon, a dla drugiego pocz <= kon
    if(a.pocz + b.pocz <= a.kon + b.kon) return wartosc_od_do2(-HUGE_VAL, HUGE_VAL);
    return wartosc_od_do2(a.pocz + b.pocz, a.kon + b.kon);
}

//mnoży ze sobą dwie liczby zmiennoprzecinkowe; automatycznie kiedy po pomnożeniu wykraczają poza hugeval lub -hugeval są przycinane, tak jak przy dodawaniu, ale...
double razy_dokladne(double a, double b){
    //...gdyby mnoźyć bez tego warunku, inf*0.0 = nan, czego nie chcemy
    if( iszero(a) || iszero(b) ) return 0.0000000000;
    return a * b;
}

void swap(wartosc *a, wartosc *b){
    wartosc p = *a;
    *a = *b;
    *b = p;
}

//zwraca sumę przedziałów a i b (w rozumieniu sumy zbiorów), z założeniem że suma ta będzie mogła zostać przedstawiona jako jeden przedział wartosc(...)
wartosc suma(wartosc a, wartosc b){
    if(pusty(a))return b;
    if(pusty(b))return a;

    if( (a.pocz <= a.kon) && (b.pocz <= b.kon) ){
        //rozpatrujemy przypadek, że jeden przedział = <..., inf>, zaś drugi = <-inf, ...>
        if(isinf(b.kon)) swap(&a,&b);
        if( isinf(a.kon) && isinf(b.pocz) ){
            if(b.kon > a.pocz) return wartosc_od_do2(-HUGE_VAL, HUGE_VAL);
            return wartosc_od_do2(a.pocz, b.kon);
        }
        //w przeciwnym przypadku przedziały a i b muszą ,,nachodzić'' na siebie w liczbie innej niż nieskończoność
        return wartosc_od_do2(min(a.pocz, b.pocz), max(a.kon, b.kon));
    }

    //zatem teraz dla któregoś z przedziałów a,b, mamy pocz > kon
    if(a.pocz > a.kon) swap(&a,&b);

    if(a.pocz <= a.kon){
        //mamy a.pocz <= a.kon ale b.pocz > b.kon
        if(a.kon >= b.pocz){
            if(a.pocz <= b.kon) return wartosc_od_do2(-HUGE_VAL, HUGE_VAL);
            return wartosc_od_do2(a.pocz, b.kon);
        }
        else{   
            //tu już wiemy że wynikiem nie będzie nieskończony przedział
            return wartosc_od_do2(b.pocz, a.kon);
        }
    }
    else{
        //a.pocz > a.kon i jednocześnie b.pocz > b.kon (bo wykonaliśmy swapa)
        if( (a.kon >= b.pocz) || (b.kon >= a.pocz) )return wartosc_od_do2(-HUGE_VAL, HUGE_VAL);
        return wartosc_od_do2(min(a.pocz, b.pocz), max(a.kon, b.kon));
    }
}

wartosc razy(wartosc a, wartosc b){
    //jeżeli któryś przedziałów jest pusty, wynik mnożenia będzie przedziałem pustym
    if(pusty(a) || pusty(b)) return wartosc_od_do2(HUGE_VAL, -HUGE_VAL);

    double t[5] = {razy_dokladne(a.pocz, b.pocz), razy_dokladne(a.pocz, b.kon), razy_dokladne(a.kon, b.pocz), razy_dokladne(a.kon, b.kon)};
    double pocz = HUGE_VAL, kon = -HUGE_VAL;
   
    if( (a.pocz <= a.kon) && (b.pocz <= b.kon) ){
        for(int i = 0; i < 4; i++){
            pocz = min(pocz, t[i]);
            kon = max(kon, t[i]);
        }
        return wartosc_od_do2(pocz, kon);
    }

    //jeżeli przedział a lub przedział b jest w postaci <...,inf> + <-inf,...> : 
    //rozbijamy go na dwa przedziały <...inf> i <-inf,...>, wykonujemy dla nich mnożenie i sumujemy zbiory wartości wyników
    //jeżeli oba przedziały są w takiej postaci, najpierw rozbijemy jeden z nich i wywołamy funkcje razy, w której ponownie rozbijemy, tym razem drugi z przedziałów
    if(a.pocz > a.kon)
        return suma( razy((wartosc_od_do2(a.pocz, HUGE_VAL)), b), razy((wartosc_od_do2(-HUGE_VAL, a.kon)), b) );
    return suma( razy((wartosc_od_do2(b.pocz, HUGE_VAL)), a), razy((wartosc_od_do2(-HUGE_VAL, b.kon)), a) );
    //w każdym przypadku sumujemy przedziały które w jakimś miejscu się ,,zazębiają", ponieważ funkcja razy zwraca jeden przedział w rozumieniu structa wartość
}

wartosc minus(wartosc a, wartosc b){
    wartosc c = razy(b, wartosc_od_do2(-1.0,-1.0));
    return plus(a,c);
}

wartosc podzielic(wartosc a, wartosc b){
    //jeżeli któryś z przedziałów a,b jest pusty, zwracamy przedział pusty
    if(pusty(a) || pusty(b)) return wartosc_od_do2(HUGE_VAL, -HUGE_VAL);
    //jeżeli b jest wartością dokładną = 0.0, wynik dzielenia będzie przedziałem pustym
    if( iszero(b.pocz) && iszero(b.kon) ) return wartosc_od_do2(HUGE_VAL, -HUGE_VAL);

    wartosc odwrotnosc;
    //zarówno jeżeli przedział b spełnia b.pocz <= b.kon, jak i dla b.pocz > b.kon, odwrotność będzie poprawnym przedziałem :
    //1.0/0.0 = inf -> tu musimy uważać, bo być może powinna wyjsć -inf, 1.0/inf = 0.0
    odwrotnosc = wartosc_od_do2(1.0/b.kon, 1.0/b.pocz);
    if(iszero(b.kon)) odwrotnosc.pocz *= -1.0;
    if( isinf(b.pocz) && isinf(b.kon) ) odwrotnosc = b;

    return razy(a, odwrotnosc);
}

