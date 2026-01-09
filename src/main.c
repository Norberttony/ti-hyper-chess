#include <ti/screen.h>
#include <ti/getcsc.h>

/* Main function, called first */
int main(void)
{
    /* Waits for a key */
    while (!os_GetCSC());

    /* Return 0 for success */
    return 0;
}
