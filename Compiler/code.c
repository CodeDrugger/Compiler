int main()
{
	int score;
	int i;
	int j;
	int avg;
	int temp;
	int e[10];;
	score = 60;
	i = 0;
	j = 0;
	while (i < 10)
	{
		e[i] = score;
		i++;
		score++;
		score++;
	}
	avg = 0;
	while (j < 10)
	{
		temp = e[j];
		avg = avg + temp;
		j++;
	}
	printf("total score is:%d",avg);
	avg = avg / 10;
	printf("avg is :%d",avg);
	return 0;
}