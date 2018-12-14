void jump(Player &j) {
  Serial.print("JUMP: ");
  Serial.println(j.y);

  j.y++; //move the player in their direction
  //  Serial.println("jump");
  Serial.print("JUMP AFTER: ");
  Serial.println(j.y);
  if ( j.y > 4) {
    j.y = 0;
  }

} //end jump


int blockPlace(int i, Player j)  {
  switch (i) {
    case 0:
      //place block at player's location
      light1_0x = j.x;
      light1_0y = j.y;
      break;
    case 1:
      light1_1x = j.x;
      light1_1y = j.y;
      break;
    case 2:
      light1_2x = j.x;
      light1_2y = j.y;
      break;
    case 3:
      light1_3x = j.x;
      light1_3y = j.y;
      break;
    case 4:
      light1_4x = j.x;
      light1_4y = j.y;
      break;
    case 5:
      light1_5x = j.x;
      light1_5y = j.y;
      break;
    case 6:
      light1_6x = j.x;
      light1_6y = j.y;
      break;

  }
}//end blockPlace
