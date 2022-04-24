unsigned int add2(unsigned int);
unsigned int add3(unsigned int);

unsigned int start(unsigned int a)
{
    a=add2(a);
    return add3(a);
}
