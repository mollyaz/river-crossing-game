#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */

char **make_river_scene(const char *left, const char* boat)
{
  char** scene = create_scene();
  add_to_scene(scene, 0,0, "bank.txt");
  add_to_scene(scene, 0,53, "bank.txt");
  add_to_scene(scene, 3,30, "sun.txt");
  add_to_scene(scene, 19,19, "river.txt");
  // print_scene(scene);

  int b_count = 0;
  int m_count = 0;
  int c_count = 0;
    
  for(int i = 0; i<7; i++)
    {if (left[i] == 'M')
	{m_count++;}

      if (left[i] == 'C')
	{c_count++;}

      if (left[i]=='B')
	{b_count++;}
    }

  int start_col = 1;
  for (int i = 0; i< m_count; i++)
    { add_to_scene(scene, 2,start_col, "missionary.txt");
      start_col+=6;}
  
   start_col = 1;
  for (int i = 0; i< c_count; i++)
    {add_to_scene(scene,11,start_col, "cannibal.txt");
      start_col+=6;}

   
   //now boat
  int m_b_count = 0;
  int c_b_count = 0;
    
  if (b_count ==1)
    {
      add_to_scene(scene, 17,19, "boat.txt");
      
      if (boat[0] == 'M')
	{add_to_scene(scene, 11,22, "missionary.txt");
	  m_b_count++;}

      if (boat[0] == 'C')
	{add_to_scene(scene, 11,22, "cannibal.txt");
	  c_b_count++;}
  
      if (boat[1] == 'M')
	{add_to_scene(scene, 11,28, "missionary.txt");
	  m_b_count++;}

      if (boat[1] == 'C')
	{add_to_scene(scene, 11,28, "cannibal.txt");
	  c_b_count++;}
    }

  if (b_count == 0)
    {    add_to_scene(scene, 17,36, "boat.txt");
      
      if (boat[0] == 'M')
	{add_to_scene(scene, 11,39, "missionary.txt");
	  m_b_count++;}

      if (boat[0] == 'C')
	{add_to_scene(scene, 11,39, "cannibal.txt");
	  c_b_count++;}
  
      if (boat[1] == 'M')
	{add_to_scene(scene, 11,45, "missionary.txt");
	  m_b_count++;}

      if (boat[1] == 'C')
	{add_to_scene(scene, 11,45, "cannibal.txt");
	  c_b_count++;}
    }

  // now right bank
  
  int right_m = 3-(m_count+ m_b_count);
  int right_c = 3-(c_count+ c_b_count);

   start_col = 54;
  for (int i = 0; i< right_m; i++)
    { add_to_scene(scene, 2,start_col, "missionary.txt");
      start_col+=6;}
  
   start_col = 54;
  for (int i = 0; i< right_c; i++)
    {add_to_scene(scene,11,start_col, "cannibal.txt");
      start_col+=6;}

return scene;
}

int perform_crossing(char* left, const char* targets)
{
  char **scene = create_scene();
  int m_count = 0;
  int c_count = 0;
  int b_count = 0;
    
  for(int i = 0; i<9; i++)
    {if (left[i] == 'M')
	{m_count++;}
      if (left[i] == 'C')
	{c_count++;}
      if (left[i]=='B')
	{b_count++;}
    }
  if (m_count >3)
    {return -2;}
  if (c_count >3)
    {return -1;}

  int t_m_count = 0;
  int t_c_count = 0;
  for(int i = 0; i<2; i++)
    {if (targets[i] == 'M')
	{t_m_count++;}
      
      if (targets[i] == 'C')
	{t_c_count++;}
    }

  if  (m_count< c_count && m_count != 0)
    {return -4;}

  int new_l_m, new_l_c;
  int p = 0;
  int j=0;
  
  cout<<"LOADED BOAT!!!!"<<endl<<endl;
    if (b_count == 1) /// this means crossing to right
      {new_l_m = m_count - t_m_count;
	new_l_c = c_count - t_c_count;
	
	if ((new_l_m < new_l_c && new_l_m != 0) || new_l_m <0 || new_l_c < 0)
	  {return -3;}
	for (int i = 0; i<10; i++)
	  left[i] = '\0';
	for(; p< new_l_m; p++)
	  {left[p] = 'M';}
	for(j = p; j< new_l_c+p; j++)
	  {left[j] = 'C';}
	left[j] = 'B';
      }
    scene = make_river_scene(left,targets);
    print_scene(scene);
    cout<<"j is: "<<j<<endl;
  
 
  cout<<"MAKE CROSSING!!!!!!"<<endl<<endl;
  cout<<"left is: "<<left<<endl;
  if (b_count == 1)
    {left[j] = ' ';}

  if (b_count == 0) /// this means crossing to left
    {int length = strlen(left);
      left[length-1] = 'B';}

  cout<<"left is now: "<<left<<endl;
  scene = make_river_scene(left,targets);
  print_scene(scene);

    
    cout<<"TRANSFER TARGETS!!!!!!!!!!!!!"<<endl<<endl;
    if (b_count == 0)
      {new_l_m = m_count + t_m_count;
	new_l_c = c_count + t_c_count;
	if ((new_l_m < new_l_c && new_l_m != 0)|| new_l_m >3 || new_l_c >3)
	  {return -3;}
  
    for (int i = 0; i<10; i++)
      left[i] = '\0';

    int p = 0;
    int j=0;
    for(; p< new_l_m; p++)
      {left[p] = 'M';}
    for(j = p; j< new_l_c+p; j++)
      {left[j] = 'C';}
    left[j] = 'B';}

    scene = make_river_scene(left,"\0");
    print_scene(scene);
    destroy_scene(scene);

    if (new_l_m < new_l_c && new_l_m != 0)
      {cout<<"new lm is less than new lc"<<endl;
	return -4;}

    int right_m = 3-(new_l_m);
    int right_c = 3-(new_l_c);

    if (right_m < right_c && right_m != 0)
      {cout<<"right m is less than right c"<<endl;
	return -4;}

    if (right_m ==3 && right_c == 3)
      {return 2;}

    else
      {	cout<<"left bank is now: "<<left<<endl;
	return 1;}
}


int play_game()
{
  // char** scene = create_scene();
  char left[10] = {"MMMCCCB"};
  // char boat[3]= {"\0"};
  char target[3]= {"\0"};
  int move=1;

  cout<<"You have 3 missionaries and 3 cannibals on left. Starting diagram is: "<<endl;
  /*
   scene = make_river_scene(left, boat);
    print_scene(scene);
    destroy_scene(scene);
  */
  cout<<"enter 1st move of 2 characters: "<<move<<endl;
  cin>> target;
  int cross_res = perform_crossing(left,target);
  cout<<status_description(cross_res)<<endl;
  cout<<cross_res<<endl;
    while(cross_res != 2 &&  cross_res != -4)
      {move++;
	cout<<"enter move number: "<<move<<"  "<<endl;
       cin>> target;
       cross_res = perform_crossing(left,target);
       cout<<"cross_res is: "<<cross_res<<endl;
       cout<<status_description(cross_res);
       cout<<endl;
       }

    cout<<"cross res is: "<<cross_res<<". exiting"<<endl;
    return cross_res;
}

// char moves[4][2] = {CC, MM, C, M, CM}
