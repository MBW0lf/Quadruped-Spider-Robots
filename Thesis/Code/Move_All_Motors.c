//Simultaneously moving all four motors in a row.
void Move_All(pwm_1, pwm_2, pwm_3, pwm_4, motor)
{
  int n, max;
  max = Find_max(pwm_1, pwm_2, pwm_3, pwm_4);
  switch(motor)
  {
     case 1:
     n = 50;
     for(;n <= max;n++)
     {
       if(pwm_3 >= n)
         htim3.Instance->CCR1 = n;
       if(pwm_1 >= n)
         htim4.Instance->CCR4 = n;
       if(pwm_4 >= n)
         htim4.Instance->CCR1 = n;
       if(pwm_2 >= n)
         htim2.Instance->CCR4 = n;
       HAL_Delay(10);
     }
     break;
  }
}