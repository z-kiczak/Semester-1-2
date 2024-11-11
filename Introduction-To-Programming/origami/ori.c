#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "stdbool.h"

typedef struct point{
    double x, y;
} point;

typedef struct shee{
    char typ; //P - prostokąt, K - koło, Z - złożenie
    double r; //promień koła
    int k;    
    point a, b; 
} shee;

point construct(double a, double b){
    point w;
    w.x = a; w.y = b;
    return w;
}

bool is_zero(double x){
    if(fabs(x) < 0.0000001) return true;
    return false;
}

//sprawdza czy a < b, z zadaną dokładnością
bool less(double a, double b){
    if(is_zero(a - b)) return false;
    if(a < b) return true;
    return false;
}

//zwraca 1 jeżeli punkt p znajduje się w prostokącie, 0 w.p.p
int in_rec(point p, shee rec){
    if( less(p.x, rec.a.x) || less(rec.b.x, p.x) ) return 0;
    if( less(p.y, rec.a.y) || less(rec.b.y, p.y) ) return 0;
    return 1;
}

//zwraca 1 jeżeli punkt p znajduje się w kole, 0 w.p.p
int in_circ(point p, shee circ){
    if( less(circ.r * circ.r, (p.x - circ.a.x) * (p.x - circ.a.x) + (p.y - circ.a.y) * (p.y - circ.a.y)) )
        return 0;
    return 1; 
}

//iloczyn wektorowy
double ilo(point p1, point p2){
   return p1.x * p2.y - p1.y * p2.x;
}

//iloczyn skalarny
double skal(point p1, point p2){
    return p1.x * p2.x + p1.y * p2.y;
}

//odejmuje wektory
point sub(point a, point b){
    return construct(a.x - b.x, a.y - b.y);
}

//dodaje wektory
point add(point a, point b){
    return construct(a.x + b.x, a.y + b.y);
}

//mnoży wektor przez skalar
point mult(point a, double c){
    return construct(a.x * c, a.y * c);
}

//rzutuje punkt na prostą
point project(point p3, shee line){
    point p1 = line.a, p2 = line.b;
    return add(mult( mult(sub(p1, p2), skal(sub(p3, p2), sub(p1, p2))), 1.0/skal(sub(p1, p2), sub(p1, p2)) ), p2);
}

//zwraca punkt który jest odbiciem symetrycznym punktu p względem prostej line [punkt p nie leży na tej prostej]
point im(point p, shee line){
    point w = project(p, line);
    return add(w, sub(w, p));
}

//zwraca liczbę przebitych warstw kartki i, przez pinezkę w punkcie p
long long int fold(point p, int i, shee* t){

    if(t[i].typ == 'Z'){
        double line_side = ilo(sub(t[i].b, t[i].a), sub(p, t[i].a));
        if( is_zero(line_side) ) return fold(p, t[i].k, t);
        if( line_side < 0.0 ) return 0LL;
        
        return fold(p, t[i].k, t) + fold( im(p, t[i]), t[i].k, t);
    }

    if( t[i].typ == 'P' ) return (long long)in_rec(p, t[i]);
    return (long long)in_circ(p, t[i]);
}

int main(){
    int n, q;
    scanf("%d %d",&n,&q);

    shee* t = (shee*)malloc(sizeof(shee) * (long unsigned int)(n + 1));

    for(int i = 1; i <= n; i++){
        scanf(" %c",&t[i].typ);

        if(t[i].typ == 'K')
            scanf("%lf %lf %lf",&t[i].a.x, &t[i].a.y, &t[i].r);
        else if(t[i].typ == 'P')
            scanf("%lf %lf %lf %lf",&t[i].a.x,&t[i].a.y,&t[i].b.x,&t[i].b.y);
        else
            scanf("%d %lf %lf %lf %lf",&t[i].k,&t[i].a.x,&t[i].a.y,&t[i].b.x,&t[i].b.y);
    }

    for(int i = 1; i <= q; i++){
        int k; point a;
        scanf("%d %lf %lf",&k,&a.x,&a.y);
        printf("%lld\n",fold(a, k, t));
    }
    free(t);
    return 0;
}