/*
Can't use wasi-libc sprintf or itoa on WASM-4 as some of the requisite syscalls aren't supported.
This was written against the clock for a game jam, don't judge it too harshly.
*/

char* inttostr(int val){
    static char buf[32] = {0};
    int count = 0,i=0, n=val;
    if(val == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return &buf[0];
    }
    while(n!=0)  
    {  
       n=n/10;
       count++;  
    }  
    count--;

    while(val!=0)  
    {  
       char digit = val%10;
       val=val/10;
       buf[count-i] = digit + 48;
       i++;  
    }

    buf[count+1] = '\0';

    return &buf[0];
}