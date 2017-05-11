int main()
{
	int a;
	int b;
	int d;
	int c[3];
	;
	a = 95;
	c[0] = a;
	c[1] = a + 1;
	c[2] = a + 2;
	for (b = 0;b < 3;b++)
	{
		d = c[b];
		a = a + d;
	}
	while (b > 0)
	{
		a++;
		b++;
	}
	return 0;
}