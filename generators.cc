#include <iostream>
#include <time.h>
#include <cmath>
#include <climits>
#include <iomanip>
#include "simlib.h"
using namespace std;

/*
 * Uvažujme funkciu F(x).
 * Napíšte pseudokód programu, ktorý vypočíta odhad plochy pod krivkou funkcie F(x) na intervale <20, 100>.
 * Je možné použiť funkcie pre generovanie pseudonáhodných čísel.
 * */

double F(double x){
    return 1.0/x;
}

double Generator1(){
    int lower = 20, upper = 100, no_attempts = 10000;
    double sum = 0, avg, x;
    for (int i = 0; i < no_attempts; ++i) {
        x = Uniform(lower, upper);
        sum += F(x);
    }
    avg = sum/(upper-lower);
    return (upper-lower)*avg;
}

/*
 * Majme kružnicu o polomere r=20.
 * Napíšte pseudokód funkcie, ktorá vypočíta obsah zadaného kruhu metódou Monte Carlo tak,
 * aby chyba metody dosahovala 5 % (zdôvodnite nastavenie presnosti metódy).
 * */

double MC_CircleContentCalculator(int r, double accuracy){
    int no_attempts = (int)((1/accuracy)*(1/accuracy)); // more attempts will give more values of PI
    double x, y, sum = 0, p, pi;
    for (int i = 0; i < no_attempts; i++) {
        x = Uniform(0, r);
        y = Uniform(0, r);
        if(x*x+y*y < r*r){
            sum++;
        }
    }
    p = sum/no_attempts;
    pi = p*4; // this gives us the approximate value of PI
    return pi*r*r;
}

/*
 * Majme guľu o polomere r=20.
 * Napíšte pseudokód funkcie, ktorá vypočíta objem zadanej gule metódou Monte Carlo tak,
 * aby chyba metody dosahovala 5 % (zdôvodnite nastavenie presnosti metódy).
 * */
double MC_SphereVolumeCalculator(int r, double accuracy) {
    int no_attempts = (int)((1/accuracy)*(1/accuracy)); // more attempts will give more values of PI
    double x, y, z, sum = 0, p, pi;
    for (int i = 0; i < no_attempts; i++) {
        x = Uniform(0, r);
        y = Uniform(0, r);
        z = Uniform(0, r);
        if(sqrt((x*x)+(y*y)+(z*z)) < r){
            sum++;
        }
    }
    p = sum/(double)no_attempts;
    pi = p*6; // this gives us the approximate value of PI TODO: 8 or 6 ???
    cout << "\nPI: " << pi << "\nNO_ATTEMPTS: " << no_attempts << endl;
    return pi*r*r*r;
}

/*
 * Majme spojitú náhodnú veličinu na intervale <0,1> danú funkciou hustoty pravdepodobnosti f(x), ktorá má rozsah funkčných hodnôt <0, 5).
 * Napíšte pseudokód generátora tejto náhodnej premennej s použitím vylučovacej metódy
 * a funkcie rnd() generujúcej rovnomerné rozloženie na intervale <0, 1>.
 * */

double Generator2(){
    double x, y;
    while (true){
        x = Random();
        y = Random()*5;
        if(y <= F(x)){
            return x;
        }
    }
}
/*
 * Bola zadana funkcia unsigned RND(), ktorá generuje pseudo nahod č.
 * Napísať pseudokód, ako zistiť dĺžku periódy kongruentného generátora tejto funkcie.
 * */

double RND(){
    return Random();
}

unsigned PeriodLength(){
    // Xn+1 = (A * Xn +B)%M
    double random = RND();
    unsigned counter = 1;
    while(RND() != random)
        counter++;
    return counter;
}

/*
 * Předpokládejte kongruentní generátor unsigned int RND();
 * Napíšte pseudokód programu, který transformuje výstup generátoru na rovnoměrné rozložení Uniform(2, 10).
 * */

unsigned int RND2(){
    return (unsigned int) Uniform(0, 100);
}

double Transform(){
    // Uniform je <2, 10). Cize potrebujeme generovat cisla <0,1), nie <0,1>.

    unsigned int rnd = RND2();
    cout << rnd << endl;
    if (rnd == 0) return 2;
    return (rnd/((double)UINT_MAX)+1)*(8)+2; // Preto je za UINT_MAX to +1... aby sa tym vylucila horna medza z daneho intervalu
}

/*
 * Uvažujme nasledujúce spojité náhodné rozloženie:
 * 20 % jeho hodnôt je rovnomerne rozložených na intervale <30, 40),
 * 80 % je rovnomerne rozložených na intervale <50, 100>.
 * Nakreslite graf jeho funkcie hustoty pravdepodobnosti s popisom oboch osí grafu.
 * Napíšte pseudokód generátora pre toto rozloženie s použitím funckie double Random()
 * vracajúcou rovnomerne rozložené hodnoty na intervale <0, 1>.
 * */

double Generator3(){
    double x, y;
    x = Random();
    y = Random();
    if (x < 0.2){
        return y*10+30;
    } else{
        return y*50+50;
    }
}

int main(){
//    cout << "Generator1: " << Generator1() << endl;
//    cout << "Generator2: " << Generator2() << endl;
//    cout << "PeriodLength: " << PeriodLength() << endl;
//    cout << "Transform: " << Transform() << endl;
    cout << "MC_CircleContentCalculator: " << MC_CircleContentCalculator(20, 0.05) << endl;
    cout << "MC_SphereVolumeCalculator: " << MC_SphereVolumeCalculator(20, 0.0001) << endl;
    return 0;
}