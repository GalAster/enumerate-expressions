// ԭ���� hqztrue��Maigo �������޸�

#include <iostream>
#include <vector>
using namespace std;

// ��ʽ�������Լ��ޡ�������������ʽ��Ŀ
long long total, neutral, pos, neg;

// �����
struct Node {
  char ch;
  Node *l, *r;
  int polar, id;
  Node(char _ch = 0, Node* _l = 0, Node* _r = 0, int _polar = 0, int _id = 0)
      : ch(_ch), l(_l), r(_r), polar(_polar), id(_id) {}
};

// �üӼ��˳����������ʽ��ȥ��
vector<Node> actions(Node& l, Node& r) {
  vector<Node> res;
  // �ӷ����������Ӷ������Ǽ��ţ����ӻ������ǼӺţ�
  //       ���Һ����ǼӺţ������Ӻ��Һ��ӵ�����Ҫ���㵥����
  if (l.ch != '+' && l.ch != '-' && r.ch != '-' && (r.ch != '+' || l.id < r.l->id)) {
    if (l.polar == 0 || r.polar == 0) {
      res.push_back(Node('+', &l, &r, l.polar + r.polar));          // �޼��� + �޼��� = �޼���
                                                                    // �м��� + �޼��� = �м����ߵļ���
    }
    else {
      res.push_back(Node('+', &l, &r, r.polar));                    // �м��� + �м��� = ����������
    }
  }
  // �������������Ӷ������Ǽ���
  if (l.ch != '-' && r.ch != '-') {
    if (l.polar == 0 && r.polar == 0) {                             // �޼��� - �޼��ԣ�
      res.push_back(Node('-', &l, &r, 1));                          // ����������������
      res.push_back(Node('-', &r, &l, -1));                         // ���������Ǹ�����
    }
    else {
      if (l.polar == 0) res.push_back(Node('-', &r, &l, r.polar));  // �м��� - �޼��� = �м����ߵļ���
                                                                    // ���޼��� - �м��� = ������
                                                                    // ���м��� - �м��� = ������
      if (r.polar == 0) res.push_back(Node('-', &l, &r, l.polar));  // ͬ��
    }
  }
  // �˷����������Ӷ������ǳ��ţ����ӻ������ǳ˺ţ�
  //       ���Һ����ǳ˺ţ������Ӻ��Һ��ӵ�����Ҫ���㵥����
  if (l.ch != '*' && l.ch != '/' && r.ch != '/' && (r.ch != '*' || l.id < r.l->id)) {
    if (l.polar == 0 || r.polar == 0) {
      res.push_back(Node('*', &l, &r, l.polar + r.polar));          // �޼��� * �޼��� = �޼���
                                                                    // �м��� * �޼��� = �м����ߵļ���
    }
    else if (l.polar > 0) {
      res.push_back(Node('*', &l, &r, r.polar));                    // ������ * �м��� = ����������
                                                                    // �������� * �м��� = ������
    }
  }
  // �������������Ӷ������ǳ���
  if (l.ch != '/' && r.ch != '/') {
    if (l.polar == 0 || r.polar == 0) {
      res.push_back(Node('/', &l, &r, l.polar + r.polar));          // �޼��� / �޼��� = �޼���
                                                                    // �м��� / �޼��� = �м����ߵļ���
                                                                    // �޼��� / �м��� = �м����ߵļ���
      res.push_back(Node('/', &r, &l, l.polar + r.polar));          // ͬ��
    }
    else {
      if (l.polar > 0) res.push_back(Node('/', &l, &r, r.polar));   // ������ / �м��� = ����������
                                                                    // �������� / �м��� = ������
      if (r.polar > 0) res.push_back(Node('/', &r, &l, l.polar));   // ͬ��
    }
  }
  return res;
}

// ö���� n ��������ɵ���ʽ
void DFS(vector<Node>& trees, int minj) {                           // trees Ϊ��ǰ��ʽ�б�minj Ϊ�ڶ�����������С�±�
  if (trees.size() == 1) {                                          // ֻʣһ����ʽ
    ++total;                                                        // ����������ʽ����
    switch (trees[0].polar) {                                       // ������Ӧ������ʽ������
      case 1: ++pos; break;
      case 0: ++neutral; break;
      case -1: ++neg; break;
    }
    if (total % 1000000 == 0) {                                     // �����û����һ����ţ�
      cout << "[Partial] Found " << total << " expressions, " << neutral << " neutral, "
           << pos << " positive, " << neg << " negative" << endl;
    }
    return;
  }

  int new_id = trees.back().id + 1;                                 // �½�� id
  for (int j = minj; j < trees.size(); ++j) {                       // ö�ٵڶ�������
    Node right = trees[j];
    trees.erase(trees.begin() + j);                                 // ���б���ȥ���ڶ�������
    for (int i = 0; i < j; ++i) {                                   // ö�ٵ�һ������
      Node left = trees[i];
      trees.erase(trees.begin() + i);                               // ���б���ȥ����һ������
      for (Node result : actions(left, right)) {                    // ö�������
        result.id = new_id;                                         // Ϊ�½�㸳�� id
        trees.push_back(result);                                    // ���б��м���������
        DFS(trees, j - 1);                                          // �ݹ���ȥ
        trees.pop_back();                                           // ���б���ȥ��������
      }
      trees.insert(trees.begin() + i, left);                        // �ѵ�һ�������ӻ��б�
    }
    trees.insert(trees.begin() + j, right);                         // �ѵڶ��������ӻ��б�
  }
}

// ������
int main(int argc, char* argv[]) {
  int n = atoi(argv[1]);                                            // �����������
  vector<Node> trees;
  for (int i = 0; i < n; ++i) {
    trees.push_back(Node('a' + i, 0, 0, 0, i));                     // ���� n ���ɵ�������ɵĳ�ʼ��ʽ
  }
  DFS(trees, 1);                                                    // ö����ʽ��ͳ������
  cout << "[ FINAL ] Found " << total << " expressions, " << neutral << " neutral, "
       << pos << " positive, " << neg << " negative" << endl;
  return 0;
}
