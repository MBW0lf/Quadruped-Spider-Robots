//Finding the maximum value among the four motors.
int Find_max(int pwm_1, int pwm_2, int pwm_3, int pwm_4)
{
	int max = pwm_1;
	if(max < pwm_2)
		max = pwm_2;
	if(max < pwm_3)
		max = pwm_3;
	if(max < pwm_4)
		max = pwm_4;
	return max;
}