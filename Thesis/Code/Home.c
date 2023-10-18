//Initial state of the robot in complete stability.
void Home()
{
  Delay 2 seconds
  Move_All(79, 75, 77, 78, 1);
  Move_All(125, 116, 117, 125, 2);
  Move_All(94, 86, 90, 85, 3); 
  Delay 1 second
}