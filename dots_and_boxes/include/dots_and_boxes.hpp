// dots_and_boxes_demo.cpp
// C++17
#include <bits/stdc++.h>
#include <cmath>
#include <error_code.h>
#include <linux/limits.h>
//#include <functional>
using namespace std;
#define DEBUG
#include <config.h>
#include <dbg.hpp>
/* @brief Move Impl
 */
struct Move {
  // [1,0]  = hor. edge / ver. edge
  bool is_hor;
  // [left, top] = 0, 0
  // hor. edge: r in [0..R], c in [0..C-1]
  // ver. edge: r in [0..R-1], c in [0..C]
  int r, c;
  //Move(bool h= true, int rr= 0, int cc= 0): is_hor(h), r(rr), c(cc) {}
  consteval Move(bool h= true, int rr= 0, int cc= 0): is_hor(h), r(rr), c(cc) {}
  string to_str() const {
    char ch= is_hor ? 'H' : 'V';
    return string(1, ch) + "(" + to_string(r) + "," + to_string(c) + ")";
  }
  // For unordered_map
  bool operator==(const Move& o) const noexcept {
    return r == o.r && c == o.c && is_hor == o.is_hor;
  }
};
/* @brief Move Hash Impl for unordered_map requirement
 */
namespace std {
template<> struct hash<Move> {
  size_t operator()(const Move& m) const noexcept {
    size_t h1= std::hash<int>{}(m.r);
    size_t h2= std::hash<int>{}(m.c);
    size_t h3= std::hash<int>{}(m.is_hor);
    // combine hashes
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};
} // namespace std
/*
 * @brief Shared data for same size Position
 */
template<int R_, int C_> struct Position_global {
  static constexpr int R= R_;
  static constexpr int C= C_;
  // num of hor. edge, 每 row C 個，共 R+1 row
  static constexpr int hor_edge_num= C * (R + 1);
  // num of ver. edge, 每 row C+1 個, 共 R row
  static constexpr int ver_edge_num= (C + 1) * R;
  static constexpr int edge_num= hor_edge_num + ver_edge_num;
  static constexpr int box_num= R * C;
  // lookup table: edge idx -> Move
  static array<Move, edge_num> edge2move;
  // lookup table: edge idx -> [left/top box idx, right/bottom box idx]
  static array<pair<int, int>, edge_num> edge2box;
  // lookup table for box idx -> [top, bottom, left, right edge]
  static array<tuple<int, int, int, int>, box_num> box2edge;
  // lookup table for box -> bitmask of surrounding edge in edges
  static array<bitset<edge_num>, box_num> boxedge_mask;
  consteval Position_global() {
    assert(R > 0 && C > 0);
    // edge2move init
    // hor.
    for(int r= 0; r <= R; ++r)
      for(int c= 0; c < C; ++c) { edge2move[hor_idx(r, c)]= Move(true, r, c); }
    // ver.
    for(int r= 0; r < R; ++r)
      for(int c= 0; c <= C; ++c) { edge2move[ver_idx(r, c)]= Move(false, r, c); }
    // edge2box init
    for(int i= 0; i < edge_num; ++i) edge2box[i]= { -1, -1 };
    for(int i= 0; i < edge_num; ++i) {
      // hor
      if(i < hor_edge_num) {
        int box_col= i % C;
        int box_row= i / C;
        // not first row
        if(box_row > 0) {
          // top box
          edge2box[i].first= (box_row - 1) * C + box_col;
        }
        // not last row
        if(box_row < R) {
          // bottom box
          edge2box[i].second= box_row * C + box_col;
        }
      }
      // ver.
      else {
        int box_col= (i - hor_edge_num) % (C + 1);
        int box_row= (i - hor_edge_num) / (C + 1);
        // not first col
        if(box_col > 0) {
          // left box
          edge2box[i].first= box_row * C + box_col - 1;
        }
        // not last col
        if(box_col < C) {
          // right box
          edge2box[i].second= box_row * C + box_col;
        }
      }
    }
    // box2edge init
    for(int br= 0; br < R; ++br) {
      for(int bc= 0; bc < C; ++bc) {
        int idx= br * C + bc;
        int top= hor_idx(br, bc);
        int bottom= hor_idx(br + 1, bc);
        int left= ver_idx(br, bc);
        int right= ver_idx(br, bc + 1);
        box2edge[idx]= { top, bottom, left, right };
      }
    }
    // boxedge_mask init
    for(int br= 0; br < R; ++br) {
      for(int bc= 0; bc < C; ++bc) {
        auto [up, bottom, left, right]= box2edge[br * C + bc];
        boxedge_mask[br * C + bc].set(up);
        boxedge_mask[br * C + bc].set(bottom);
        boxedge_mask[br * C + bc].set(left);
        boxedge_mask[br * C + bc].set(right);
      }
    }
    DOUT << "DBG: Poisition Glocal Init Done" << endl;
  }
  /*
   * @brief Box idx to hor. edge idx
   * @param r range [0..R]
   * @param c range [0..C-1]
   */
  inline int hor_idx(int r, int c) const { return r * C + c; }
  /*
   * @brief Box idx to ver. edge idx
   * @param r range [0..R-1]
   * @param c range [0..C]
   */
  inline int ver_idx(int r, int c) const { return hor_edge_num + r * (C + 1) + c; }
};
// C++ static init
template<int R, int C>
array<Move, Position_global<R, C>::edge_num> Position_global<R, C>::edge2move= {};
template<int R, int C>
array<pair<int, int>, Position_global<R, C>::edge_num>
    Position_global<R, C>::edge2box= {};
template<int R, int C>
array<tuple<int, int, int, int>, Position_global<R, C>::box_num>
    Position_global<R, C>::box2edge= {};
template<int R, int C>
array<bitset<Position_global<R, C>::edge_num>, Position_global<R, C>::box_num>
    Position_global<R, C>::boxedge_mask= {};
/* @brief Position Impl
 */
template<typename Position_global> class Position {
public:
  // convenience
  static constexpr const int& R= Position_global::R;
  static constexpr const int& C= Position_global::C;
  static constexpr auto& hor_edge_num= Position_global::hor_edge_num;
  static constexpr auto& ver_edge_num= Position_global::ver_edge_num;
  static constexpr auto& edge_num= Position_global::edge_num;
  static constexpr auto& edge2move= Position_global::edge2move;
  static constexpr auto& edge2box= Position_global::edge2box;
  static constexpr auto& box2edge= Position_global::box2edge;
  static constexpr auto& box_num= Position_global::box_num;
  // edges exist, hor. first, [0,1] = not exist / exist
  bitset<Position_global::edge_num> edge_exist;
  // box owner, [0,1] = player 1 / player 2
  bitset<Position_global::box_num> box_owner;
  // [0,1] = player 1 / player 2
  bool cur_player;
  // lookup table
  // empty edge
  bitset<Position_global::edge_num> edge_empty;
  // num of edge surrounding ith box
  int box_edge_num[Position_global::box_num]= { 0 };
  // num of box with ith edge
  int n_edge_box_num[5]= { 0 };
  // num of box of player
  int player_box_num[2]= { 0 };
  //int
  //remain_edge
  /*
   * @brief Constructor
   * @param rows = 2, number of box rows
   *
   * @param cols = 2, number of box columns
   * @param startPlayer = 1, 1 for player first, -1 for opponent first
   */
  Position(int cur_player= 0): cur_player(cur_player) {
    edge_empty.set();
    // The box with 0 edge is all
    n_edge_box_num[0]= Position_global::box_num;
  }
  Position(const Position& o)
      : edge_exist(o.edge_exist), edge_empty(o.edge_empty), box_owner(o.box_owner),
        cur_player(o.cur_player) {
    memcpy(box_edge_num, o.box_edge_num, Position_global::box_num * sizeof(int));
    memcpy(player_box_num, o.player_box_num, 2 * sizeof(int));
    memcpy(n_edge_box_num, o.n_edge_box_num, 5 * sizeof(int));
  }
  /* @brief Box idx to hor. edge idx
   */
  inline int hor_idx(int r, int c) const { return r * C + c; }
  /* @brief Box idx to ver. edge idx
   */
  inline int ver_idx(int r, int c) const { return hor_edge_num + r * (C + 1) + c; }
  inline int get_move2idx(const Move& m) {
    if(m.is_hor)
      return hor_idx(m.r, m.c);
    else
      return ver_idx(m.r, m.c);
  }
  /* @brief Set edge by m
   */
  inline void set_edge(const Move& m) {
    int idx= m.is_hor ? hor_idx(m.r, m.c) : ver_idx(m.r, m.c);
    assert(edge_num > idx && idx >= 0);
    edge_exist.set(idx);
  }
  inline void set_edge(const int idx) {
    Position_global assert(edge_num > idx && idx >= 0);
    edge_exist.set(idx);
  }
  /* @brief Check whether the game terminated
   */
  inline bool is_terminal() const {
    return (player_box_num[0] + player_box_num[1]) == Position_global::box_num;
  }
  /* @brief Return 1 if is hor. edge
   */
  inline bool is_hor_edge(int idx) { return idx < hor_edge_num; }
  /* @brief Check the box edge is full
   */
  inline bool check_box(int box_idx) {
    if(box_idx < 0) return false;
    return (edge_exist & Position_global::boxedge_mask[box_idx]) ==
           Position_global::boxedge_mask[box_idx];
  }
  /*
   * @brief Apply move, set edge and update box
   * @return num of box gained
   */
  int apply_move(const int idx) {
    //if(edge_exist.test(idx)) return -1;
    edge_exist.set(idx);
    edge_empty.reset(idx);
    int gained_box= 0;
    auto [b1, b2]= Position_global::edge2box[idx];
    if(b1 >= 0) {
      // update lookup tables
      --n_edge_box_num[box_edge_num[b1]];
      ++box_edge_num[b1];
      ++n_edge_box_num[box_edge_num[b1]];
      if(check_box(b1)) {
        ++gained_box;
        box_owner[b1]= cur_player;
        ++player_box_num[cur_player];
      }
    }
    if(b2 >= 0) {
      --n_edge_box_num[box_edge_num[b2]];
      ++box_edge_num[b2];
      ++n_edge_box_num[box_edge_num[b2]];
      if(check_box(b2)) {
        ++gained_box;
        box_owner[b2]= cur_player;
        ++player_box_num[cur_player];
      }
    }
    // update player to play
    if(gained_box == 0) {
      if(cur_player == 0)
        cur_player= 1;
      else
        cur_player= 0;
    }
    return gained_box;
  }
  /* @brief Return score
   */
  int get_score() const {
    return (cur_player == 0) ? player_box_num[0] - player_box_num[1]
                             : -(player_box_num[0] - player_box_num[1]);
  }
  /* @brief printer
   */
  void print() const {
    // layout: dots and horiz edges then vertical edges and box owners
    for(int dr= 0; dr <= R; ++dr) {
      // horizontal line row
      for(int dc= 0; dc < C; ++dc) {
        cout << "•";
        cout << (edge_exist[hor_idx(dr, dc)] ? "──" : "  ");
      }
      cout << "•\n";
      if(dr < R) {
        // vertical edges + box owner row
        for(int dc= 0; dc <= C; ++dc) {
          cout << (edge_exist[ver_idx(dr, dc)] ? "│" : " ");
          if(dc < C) {
            if(box_edge_num[dr * C + dc] == 4) {
              int owner= box_owner[dr * C + dc];
              if(owner == 0)
                cout << "1 ";
              else if(owner == 1)
                cout << "2 ";
            } else {
              cout << "  ";
            }
          }
        }
        cout << "\n";
      }
    }
  }
  int apply_move(const Move& m) { return apply_move(get_move2idx(m)); }
};
/* @brief The record across multiple position
 */
template<typename Position_global> class Record {
public:
  static constexpr const int& R= Position_global::R;
  static constexpr const int& C= Position_global::C;
  // Initial position
  Position<Position_global> init;
  // Player Sequence
  vector<int> player_seq;
  // Move Seqeunce
  vector<Move> moves;
  // Current playing position
  Position<Position_global> cur;
  Record(const Position<Position_global>& startPos): init(startPos), cur(startPos) {}
  /* @brief Add move for position, and record this move
   */
  ERR add_move(const Move& m) {
    //int score =
    cur.apply_move(m);
    moves.push_back(m);
    player_seq.push_back(cur.cur_player);
    return ERR::OK;
  }
};
/* @brief AI Impl
 */
template<typename Position_global> class Alphabeta {
public:
  int maxDepth;
  Alphabeta(int md= 6): maxDepth(md) {}
  /* @breif Evaluate position
   * TODO: Need better heuristic evaluate function
   */
  static int evaluate(const Position<Position_global>& pos) {
    // deterministic: score
    int base= pos.get_score();
    int n3= pos.n_edge_box_num[3];
    int sign= pos.cur_player == 0 ? 1 : -1;
    return base * 100 + (-sign) * n3 * 7;
  }
  static bool big_cmp(
      const Position<Position_global>& a, const Position<Position_global>& b) {
    return evaluate(a) > evaluate(b);
  }
  static bool sml_cmp(
      const Position<Position_global>& a, const Position<Position_global>& b) {
    return evaluate(a) < evaluate(b);
  }
  /* alpha beta searching
   */
  int alphabeta(const Position<Position_global>& pos, int depth, int alpha, int beta) {
    // no game
    if(pos.is_terminal()) { return pos.get_score(); }
    // no move available
    if(pos.edge_empty.none()) { return pos.get_score(); }
    // no depth
    if(depth == 0) { return evaluate(pos); }
    // create candidate edge and pos
    vector<int> cand_edge;
    cand_edge.reserve(Position_global::edge_num);
    for(size_t idx= pos.edge_empty._Find_first(), cnt= 0; idx < pos.edge_empty.size();
        idx= pos.edge_empty._Find_next(idx), ++cnt) {
      cand_edge.emplace_back(idx);
    }
    vector<Position<Position_global>> cand_pos(
        cand_edge.size(), pos); // 大批拷貝發生！怎麼避免？
    for(int i= 0; i < (int)cand_edge.size(); ++i) {
      cand_pos[i].apply_move(cand_edge[i]);
    }
    // minmax: max
    if(pos.cur_player == 0) {
      sort(cand_pos.begin(), cand_pos.end(), big_cmp);
      int value= numeric_limits<int>::min();
      for(auto cur_pos: cand_pos) {
        int val= alphabeta(cur_pos, depth - 1, alpha, beta);
        value= max(value, val);
        alpha= max(alpha, value);
        if(alpha >= beta) { break; }
      }
      return value;
    } else // minmax: min
    {
      sort(cand_pos.begin(), cand_pos.end(), sml_cmp);
      int value= numeric_limits<int>::max();
      for(size_t idx= pos.edge_empty._Find_first(); idx < pos.edge_empty.size();
          idx= pos.edge_empty._Find_next(idx)) {
        Position next= pos;
        // auto score =
        next.apply_move(idx);
        int val= alphabeta(next, depth - 1, alpha, beta);
        value= min(value, val);
        vector<pair<int, int>> moves; // pair<score, idx>
        beta= min(beta, value);
        if(alpha >= beta) break;
      }
      return value;
    }
  }
  /*
   * @brief Find best move of position
   * Top layer of alphabeta dfs
   * @return Best move and evaluation
   */
  pair<Move, int> best_move(const Position<Position_global>& pos) {
    int bestVal;
    // init max layer value
    if(pos.cur_player == 0)
      bestVal= numeric_limits<int>::min();
    else
      bestVal= numeric_limits<int>::max();
    int alpha= numeric_limits<int>::min();
    int beta= numeric_limits<int>::max();
    Move bestMove;
    for(size_t idx= pos.edge_empty._Find_first(); idx < pos.edge_empty.size();
        idx= pos.edge_empty._Find_next(idx)) {
      Position next= pos;
      next.apply_move(idx);
      int val= alphabeta(next, maxDepth - 1, alpha, beta);
      if(pos.cur_player == 0) {
        if(val > bestVal) {
          bestVal= val;
          bestMove= Position_global::edge2move[idx];
          alpha= max(alpha, bestVal);
        }
      } else {
        if(val < bestVal) {
          bestVal= val;
          bestMove= Position_global::edge2move[idx];
          beta= min(beta, bestVal);
        }
      }
      if(alpha >= beta) break;
    }
    return { bestMove, bestVal };
  }
};
