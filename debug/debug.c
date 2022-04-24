
__attribute__((import_module("env"), import_name("conlogx"))) void conlogx(unsigned int,unsigned int);

char *dpbuf;

void conlog(unsigned int a,unsigned int b)
{
    conlogx(a,b);
}