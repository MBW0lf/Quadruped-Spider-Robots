  //Initialize all configured peripherals
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  //For leg1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //Pin A8 => Motor Number 3
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4); //Pin B1 => Motor Number 2
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4); //Pin B9 => Motor Number 1
  //For leg2
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //Pin A1 => Motor Number 3
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); //Pin A2 => Motor Number 2
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4); //Pin A3 => Motor Number 1
  //For leg3
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //Pin A6 => Motor Number 1
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); //Pin A7 => Motor Number 2
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); //Pin B0 => Motor Number 3
  //For leg4
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1); //Pin B6 => Motor Number 1
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); //Pin B7 => Motor Number 2
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3); //Pin B8 => Motor Number 3