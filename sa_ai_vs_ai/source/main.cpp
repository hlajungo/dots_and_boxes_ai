#include <bm.h>
#include <dots_and_boxes.hpp>
auto measure_ms= [](auto func) -> size_t {
  auto start= chrono::high_resolution_clock::now();
  func();
  auto end= chrono::high_resolution_clock::now();
  return chrono::duration_cast<chrono::milliseconds>(end - start).count();
};
auto measure_ms_with_ret= [](auto& duration_ms, auto func) {
  using namespace std::chrono;
  auto start= high_resolution_clock::now();
  auto result= func(); // 執行函數並保存返回值
  auto end= high_resolution_clock::now();
  duration_ms= duration_cast<milliseconds>(end - start).count();
  return result; // 返回函數原本的返回值
};
/*
 * @brief alphabeta minmax AI vs AI demo
 */
Position_global<3, 2> pg;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  auto all_time= measure_ms([&]() {
    Position<decltype(pg)> pos(0); // player1 先手
    Record<decltype(pg)> rec(pos);
    //Alphabeta ai(pg.edge_num - 1); // 搜尋深度（視狀況調整；過深會耗很久）
    //Alphabeta<decltype(pg)> ai(pg.edge_num);
    // 3x2 = 17, 2x2=12, 3x3=24
    Alphabeta<decltype(pg)> ai(9);
    cout << "Start dots and boxes with " << pg.R << "x" << pg.C << " boxes" << endl;
    pos.print();
    cout << endl;
    while(!rec.cur.is_terminal()) {
      // At this spot, the cur_player has been change to opponent
      cout << "Player " << (rec.cur.cur_player == 0 ? "1" : "2");
      size_t duration;
      auto [mv, val]=
          measure_ms_with_ret(duration, [&]() { return ai.best_move(rec.cur); });
      rec.add_move(mv);
      cout << " plays " << mv.to_str() << ", eval = " << val << ", time = " << duration
           << endl;
      rec.cur.print();
      cout << flush;
      // 短暫防止過多輸出
      //this_thread::sleep_for (chrono::milliseconds (200));
    }
    cout << "Game over.\n";
    rec.cur.print();
    cout << "Final score (player1 - player2) = " << rec.cur.get_score() << "\n";
    cout << "player1 boxes = " << rec.cur.player_box_num[0] << "\n";
    cout << "player2 boxes = " << rec.cur.player_box_num[1] << "\n";
  });
  cout << "Match time: " << all_time << endl;
  return 0;
}
