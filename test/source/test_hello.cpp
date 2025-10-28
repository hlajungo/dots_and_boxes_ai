#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// #include <doctest.h>
#include <config.h>
#include <doctest/doctest.h>
#include <dots_and_boxes.hpp>
#include <hello.h>
Position_global<2, 2> pg;
TEST_CASE ("Position_global")
{
  CHECK (pg.R == 2);
  CHECK (pg.C == 2);
  CHECK (pg.hor_edge_num == 6);
  CHECK (pg.ver_edge_num == 6);
  CHECK (pg.edge_num == 12);
  vector<tuple<int, int, int, int> > ans1
      = { { 0, 2, 6, 7 }, { 1, 3, 7, 8 }, { 2, 4, 9, 10 }, { 3, 5, 10, 11 } };
  CHECK (ans1.size () == pg.box2edge.size ());
  for (int i = 0; i < (int)pg.box2edge.size (); ++i)
  {
    CHECK (ans1[i] == pg.box2edge[i]);
  }
  vector<pair<int, int> > ans2
      = { { -1, 0 }, { -1, 1 }, { 0, 2 },  { 1, 3 },  { 2, -1 }, { 3, -1 },
          { -1, 0 }, { 0, 1 },  { 1, -1 }, { -1, 2 }, { 2, 3 },  { 3, -1 } };
  CHECK (ans2.size () == pg.edge2box.size ());
  for (int i = 0; i < (int)pg.edge2box.size (); ++i)
  {
    CHECK (ans2[i] == pg.edge2box[i]);
  }
  vector<Move> ans3
      = { { true, 0, 0 },  { true, 0, 1 },  { true, 1, 0 },  { true, 1, 1 },
          { true, 2, 0 },  { true, 2, 1 },  { false, 0, 0 }, { false, 0, 1 },
          { false, 0, 2 }, { false, 1, 0 }, { false, 1, 1 }, { false, 1, 2 } };
  for (int i = 0; i < (int)pg.edge2move.size (); ++i)
  {
    CHECK (ans3[i] == pg.edge2move[i]);
  }
}
TEST_CASE ("Position")
{
  Position<Position_global<2, 2> > pos (0);
  CHECK (pos.edge_exist.size () == pg.edge_num);
  CHECK (pos.edge_empty.size () == pg.edge_num);
  CHECK (pos.box_owner.size () == pg.box_num);
  CHECK (pos.cur_player == 0);
  // None of move played
  for (int i = 0; i < pos.edge_num; ++i)
  {
    CHECK (pos.edge_exist.test (i) == 0);
    CHECK (pos.edge_empty.test (i) == 1);
  }
  for (int i = 0; i < pg.box_num; ++i)
  {
    CHECK (pos.box_owner.test (i) == 0);
  }
  for (int i = 0; i < pos.edge_num; ++i)
  {
    int old_player = pos.cur_player;
    int gained_box = pos.apply_move (i);
    if (gained_box != 0)
      CHECK (old_player == pos.cur_player);
    else
      CHECK (old_player != pos.cur_player);
    CHECK (pos.edge_exist.test (i) == 1);
    CHECK (pos.edge_empty.test (i) == 0);
    //for (size_t idx = pos.edge_empty._Find_first ();
         //idx < pos.edge_empty.size ();
         //idx = pos.edge_empty._Find_next (idx))
    //{
    //}
    // last move end the game
    if (i == pg.edge_num - 1)
    {
      //pos.print();
      //cout << pos.player_box_num[0] << pos.player_box_num[1] << endl;
      CHECK (pos.is_terminal () == true);
    }
    else
    {
      //pos.print();
      //cout << pos.player_box_num[0] << pos.player_box_num[1] << endl;
      CHECK (pos.is_terminal () == false);
    }
  }
  // All of move played
  CHECK (pos.edge_exist.count () == pg.edge_num);
  CHECK (pos.edge_empty.none () == true);
}
//TEST_CASE ("Position Function")
//{
//// hor_idx & ver_idx
//int R = 3, C = 2;
//Position_global pg (R, C);
//Position pos (pg, 0);
//for (int i = 0; i <= R; ++i)
//for (int j = 0; j < C; ++j)
//CHECK (pos.hor_idx (i, j) == i * pos.C + j);
//for (int i = 0; i < R; ++i)
//for (int j = 0; j <= C; ++j)
//CHECK (pos.ver_idx (i, j) == pos.hor_edge_num + i * (pos.C + 1) + j);
//// set_edge & is_terminal
//// set_edge hor
//Position pos2 (R, C, 1);
//for (int i = 0; i <= R; ++i)
//for (int j = 0; j < C; ++j)
//CHECK (pos2.set_edge (true, i, j) == true);
//// set hor again
//for (int i = 0; i <= R; ++i)
//for (int j = 0; j < C; ++j)
//CHECK (pos2.set_edge (true, i, j) == false);
//// here set C, not C-1, so later can check is_terminal
//for (int i = 0; i < R; ++i)
//for (int j = 0; j < C; ++j)
//CHECK (pos2.set_edge (false, i, j) == true);
//for (int i = 0; i < R; ++i)
//for (int j = 0; j < C; ++j)
//CHECK (pos2.set_edge (false, i, j) == false);
//CHECK (pos2.is_terminal () == false);
//// last vertical edge set
//for (int i = 0; i < R; ++i)
//{
//CHECK (pos2.set_edge (false, i, C) == true);
//}
//// all edges set
//CHECK (pos2.is_terminal () == true);
//// box_edge_num & edge_remain & n_edge_box_num
//// all edge set, so all 4
//for (int i = 0; i < R; ++i)
//for (int j = 0; j < C; ++j)
//CHECK (pos2.box_edge_num (i, j) == 4);
//CHECK (pos2.edge_remain () == 0);
//// Total 3x2 box
//CHECK (pos2.n_edge_box_num (4) == 6);
//CHECK (pos2.n_edge_box_num (3) == 0);
//CHECK (pos2.n_edge_box_num (2) == 0);
//CHECK (pos2.n_edge_box_num (1) == 0);
//CHECK (pos2.n_edge_box_num (0) == 0);
//}
//TEST_CASE ("Move Constructor")
//{
//Move move (true, 0, 0);
//CHECK (move.to_str () == "H(0,0)");
//}
//TEST_CASE ("Position apply_move")
//{
//int R = 3, C = 2;
//Position pos (R, C, 1);
//int cur_player = 1;
//// playing all horizontal
//for (int i = 0; i <= R; ++i)
//{
//for (int j = 0; j < C; ++j)
//{
//Move move (true, i, j);
//auto ret = pos.apply_move (move, cur_player);
//if (ret == 0)
//{
//CHECK (pos.cur_player == -cur_player);
//cur_player = -cur_player;
//}
//else
//{
//CHECK (pos.cur_player == cur_player);
//}
////pos.print ();
////cout << "-----\n";
//}
//}
//// checking all horizontal
//for (int i = 0; i <= R; ++i)
//{
//for (int j = 0; j < C; ++j)
//{
//CHECK (pos.edges[pos.hor_idx (i, j)] == true);
//}
//}
//// checking all vertical
//for (int i = 0; i < R; ++i)
//{
//for (int j = 0; j <= C; ++j)
//{
//CHECK (pos.edges[pos.ver_idx (i, j)] == false);
//}
//}
//// playing all vertical
//for (int i = 0; i < R; ++i)
//{
//for (int j = 0; j <= C; ++j)
//{
//Move move (false, i, j);
//auto ret = pos.apply_move (move, cur_player);
//if (ret == 0)
//{
//CHECK (pos.cur_player == -cur_player);
//cur_player = -cur_player;
//}
//else
//{
//CHECK (pos.cur_player == cur_player);
//}
//pos.print ();
//cout << "-----\n";
//}
//}
//// checking all horizontal
//for (int i = 0; i <= R; ++i)
//{
//for (int j = 0; j < C; ++j)
//{
//CHECK (pos.edges[pos.hor_idx (i, j)] == true);
//}
//}
//// checking all vertical
//for (int i = 0; i < R; ++i)
//{
//for (int j = 0; j <= C; ++j)
//{
//CHECK (pos.edges[pos.ver_idx (i, j)] == true);
//}
//}
//CHECK (pos.box_own_num (1) == 2);
//CHECK (pos.box_own_num (-1) == 4);
//CHECK (pos.score () == -2);
//}
/*
 * @brief Move with Record
 */
//TEST_CASE ("Record add_move")
//{
//int R = 3, C = 2;
//Position pos (R, C, 1);
//Record rec (pos);
//int cur_player = 1;
//// Playing all horizontal edge
//for (int i = 0; i <= R; ++i)
//{
//for (int j = 0; j < C; ++j)
//{
//Move move (true, i, j);
//auto ret = rec.add_move (move, rec.cur.cur_player);
//// add_move will change internal cur player, change this var for next CHECK
//cur_player = -cur_player;
//CHECK (ret == true);
//CHECK (rec.cur.cur_player == cur_player);
////rec.cur.print ();
//}
//}
//}
