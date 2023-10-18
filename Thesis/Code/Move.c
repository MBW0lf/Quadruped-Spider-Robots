//Moving the motor to a desired angle.
void Move(htim, timer, CCR, pwm, motor)
{
  Store the instantaneous value of the motor in variable (n)
  if(Is the previous value less than the new value or not)
  {
    switch(CCR)
    {
      case 1:
        for(;n <= pwm;n++)
        {
          htim.Instance->CCR1 = n;
          HAL_Delay(10);
        }
        break;
      case 2:
        for(;n <= pwm;n++)
        {
          htim.Instance->CCR2 = n;
          HAL_Delay(10);
        }
        break;
      case 3:
        for(;n <= pwm;n++)
        {
          htim.Instance->CCR3 = n;
          HAL_Delay(10);
        }
        break;
      case 4:
        for(;n <= pwm;n++)
        {
          htim.Instance->CCR4 = n;
          HAL_Delay(10);
        }
        break;
    }
  }
  Store the new value
}