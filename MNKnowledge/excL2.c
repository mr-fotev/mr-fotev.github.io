#include <stdio.h>
#include <math.h>
#define M 1000000

int main(){
    double a, b, t, d;
    
    while (scanf("%lf %lf %lf %lf", &a, &b, &t, &d) > 0){
        double step = 1/t, s = (a-b-d)*step, e = 2*d*step, cs, fs;
    
        int intJump = floor(t);
        
        int i = 0;
        while (i <= M){
            fs = s + step*i;
            cs = ceil(fs);
            if (fs + e > cs) break;
            else i += (fs + step > cs ? intJump:1);
        }
        
        printf(i <= M ? "%.3lf\n":"We are safe\n", i+a);
    }
    
    return 0;
}
