#include <stdio.h>
#include <conio.h>

int main(void) {
    int c;

    printf("Press any key\n");
    c = getch();
    printf("You pressed char %c decimal %d\n", c, c);
    return 0;
}
