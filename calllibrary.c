#include "bimconv.hh"

extern "C" int my_function(int num_numbers, int *numbers) {
    int i;
    int sum;
    for (i = 0; i < num_numbers; i++) {
        sum += numbers[i];
    }
      throw 20;
    return sum;
}

int main(){
    printusage("HOLA");

    return 0;
}
