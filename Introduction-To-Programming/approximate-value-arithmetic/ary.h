#ifndef _ARY_H_
#define _ARY_H_

#include "stdbool.h"

bool iszero(double x);

typedef struct wartosc {
  /* uzupelnij! */
  double pocz, kon;
  //jeśli pocz <= kon, przedzial = <pocz,kon>
  //jesli pocz > kon, przedzial = <pocz, HUGE_VAL> + <-HUGE_VAL, kon>
  //przedzial <-HUGE_VAL, HUGE_VAL> -> zawiera wszystkie liczby rzeczywiste <-inf, inf>; jest on różny od przedziału <x,x>, który zawiera dokładnie jeden element, x
  //przedział <HUGE_VAL, -HUGE_VAL> -> reprezentacja zbioru/przedziału pustego
} wartosc;

/* Implicite zakładamy, że wszystkie argumenty typu double są liczbami  */
/* rzeczywistymi, tzn. są różne od HUGE_VAL, -HUGE_VAL i NAN. */

/* wartosc_dokladnosc(x, p, &wynik) ustawia w na x +/- p% */
/* warunek początkowy: p > 0                              */
wartosc wartosc_dokladnosc(double x, double p);

/* wartosc_od_do(x,y,&wynik) ustawia wynik na [x;y]       */
/* warunek poczatkowy: x <= y                             */
wartosc wartosc_od_do(double x, double y);

/* wartosc_dokladna(x, &wynik) ustawia wynik na [x;x]     */
wartosc wartosc_dokladna(double x);

/* in_wartosc(&w, x) = x \in w */
bool in_wartosc(wartosc w, double x);

/* min_wartosc(&w) = najmniejsza możliwa wartość w,       */
/* lub -HUGE_VAL jeśli brak dolnego ograniczenia.         */
double min_wartosc(wartosc w);

/* max_wartosc(&w) = największa możliwa wartość w,        */
/* lub HUGE_VAL jeśli brak górnego ograniczenia.          */
double max_wartosc(wartosc w);

/* środek przedziału od min_wartosc do max_wartosc,       */
/* lub nan jeśli min i max_wartosc nie są określone.      */
double sr_wartosc(wartosc w);

/* Operacje arytmetyczne na niedokładnych wartościach.    */
wartosc plus(wartosc a, wartosc b);
wartosc minus(wartosc a, wartosc b);
wartosc razy(wartosc a, wartosc b);
wartosc podzielic(wartosc a, wartosc b);

#endif