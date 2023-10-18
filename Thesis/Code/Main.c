switch(Which color detected?)
{
  if(The obstacle is Pink) 
    if(The area of the obstacle has exceeded the limit or not?)
      Forward(1);
    else
    {
       Turn_Right(5);
       Forward(11);
       Turn_Left(5);
       Forward(11);
       Turn_Right(5);
    }
  if(The obstacle is Blue)
    if(The area of the obstacle has exceeded the limit or not?)
      Forward(1);
    else
      Forward(20);
}