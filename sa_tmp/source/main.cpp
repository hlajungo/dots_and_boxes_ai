#include <dots_and_boxes.hpp>

auto measure_ms = [] (auto func) -> size_t
{
  auto start = chrono::high_resolution_clock::now ();
  func ();
  auto end = chrono::high_resolution_clock::now ();
  return chrono::duration_cast<chrono::milliseconds> (end - start).count ();
};

auto measure_ms_with_ret = [] (auto& duration_ms, auto func)
{
  using namespace std::chrono;
  auto start = high_resolution_clock::now ();
  auto result = func (); // 執行函數並保存返回值
  auto end = high_resolution_clock::now ();
  duration_ms = duration_cast<milliseconds> (end - start).count ();
  return result; // 返回函數原本的返回值
};

/*
 * @brief alphabeta minmax AI vs AI demo
 */
int
main ()
{
  Position_global global(2,2);
  Position pos(global, 0);
  auto score = pos.apply_move(0);
  score = pos.apply_move(2);
  score = pos.apply_move(6);
  score = pos.apply_move(7);
  pos.print();
  cout << score << endl;
  return 0;
}
