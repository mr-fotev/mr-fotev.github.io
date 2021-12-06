#include <stdio.h>
#include <math.h>
#define M 1000000

int main(){
    double a, b, t, d;
    
    while (scanf("%lf %lf %lf %lf", &a, &b, &t, &d) > 0){
        double step = 1/t, s = (a-b-d)*step, e = 2*d*step, cs;
    
        int intJump = floor(t);
        double floatJump = intJump*step;
        
        int i = 0;
        while (i <= M){
            cs = ceil(s);
            if (s + e > cs) break;
            else if (s + step > cs) s += floatJump, i += intJump;
            else s += step, i++;
        }
        
        printf(i <= M ? "%.3lf\n":"We are safe\n", i+a);
    }
    
    return 0;
}
