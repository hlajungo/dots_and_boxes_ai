//class MCTS_node {
//public:
  //Position pos;
  //int visited= 0;
  //int wins= 0;
  //const double C= 1.4;
  //MCTS_node* parent;
  //vector<MCTS_node*> children;
  //MCTS_node() {
    //for(auto move: pos.legal_moves()) {
      //int gained_box;
      //Position tmp= pos;
      //tmp.apply_move(gained_box, move);
      //MCTS_node* ptr= new MCTS_node(tmp);
      //children.push_back(ptr);
    //}
  //}
  //MCTS_node(Position pos): pos(pos) {}
  //double ucb() {
    //if(visited == 0) return 1e9;
    //return (double)wins / visited + C * sqrt(log(parent->visited) / visited);
  //}
//};
//class MCTS_head {
//public:
//MCTS_node MCTS_head;
/*
   * @brief Get best child of node
   */
//MCTS_node* select(MCTS_node* n) {
//while(!n->children.empty()) {
//MCTS_node* best= nullptr;
//double bestVal= -1e9;
//for(auto c: n->children) {
//double val= c->ucb();
//if(val > bestVal) {
//bestVal= val;
//best= c;
//}
//}
//n= best;
//}
//return n;
//}
//MCTS_node* expand(MCTS_node* n) {
//if(n->unexpandedMoves.empty()) return n; // 不能展開
//int m= n->unexpandedMoves.back();
//n->unexpandedMoves.pop_back();
//vector<char> s= n->state;
//s[m]= n->player;
//Node* child= new Node(s, (n->player == 'X') ? 'O' : 'X', n);
//n->children.push_back(child);
//return child;
//}
//};
//// 選擇子節點
//class MCTS {
//public:
//};

