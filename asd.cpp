#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define EL endl

//template <size_t N>
//bool is[N + 1];

//template <size_t N>
//struct Prime
//{
//template <size_t N2>
//bool is[N + 1];
//constexpr
//Prime ()
//{
//is<N>[0] = is<N>[1] = false;
//for (size_t i = 2; i <= N; i++)
//is<N>[i] = true;
//for (size_t i = 2; i * i <= N; i++)
//if (is<N>[i])
//for (size_t j = i * i; j <= N; j += i)
//is<N>[j] = false;
//}
//};
template <size_t N>
struct Prime2
{
  static bool is[N + 1]; // Forward declaration of template entity is here 

  constexpr
  Prime2 ()
  {
    for (size_t i = 0; i <= N; ++i)
      is[i] = true;
    is[0] = is[1] = false;
    for (size_t i = 2; i * i <= N; ++i)
    {
      if (is[i])
        for (size_t j = i * i; j <= N; j += i)
          is[j] = false;
    }
  }
};
template <size_t N>
bool Prime2<N>::is[N + 1] = {};

int
main ()
{
  ios_base::sync_with_stdio (0);
  cin.tie (0);
  cout.tie (0);
  Prime2<100> p2;
  std::cout << Prime2<100>::is[5]; // Instantiation of variable 'Prime2<100>::is' required here, but no definition is available 
  std::cout << Prime2<100>::is[6]; // 0

  //cout << is<100>[5]; // 1
  //cout << is<100>[6]; // 0
  //cout << is<99>[5]; // 0
}
