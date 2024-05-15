#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "preprocess.h"
#include "Excep_syntax.h"
#include "Token_stream.h"
#include "Excep_dev.h"
#include "Excep_syntax.h"

using namespace std;

static string PRE_OUTPUT_FILE = "file.pre";     // 预处理输出文件的名称



template <typename T>

void unite(vector<T>& t1, const vector<T>& t2)
{
    for (const auto& t : t2) {
        if (!my_std::is_in(t, t1)) t1.emplace_back(t);
    }
}

template <typename T>
// 如果 t1 与 t2 有相交的部分，则 t1 = t1 并 t2，返回 true
// 否则 t1 不变，返回 false
static bool unite_reliance(vector<T>& t1, const vector<T>& t2)
{
    bool flag = false;
    for (const auto& t : t1) {
        if (my_std::is_in(t, t2)) {
            flag = true;
            break;
        }
    }
    if (flag) unite(t1, t2);
    return flag;
}


// 找出 ts 中第一个不存在 target 中的元素下表，用于提示错误信息
static int index_first_not_find(const vector<htf::path_deal::Hpath>& ts, const vector<htf::path_deal::Hpath>& target)
{
    int i = ts.size() - 1;
    for (; i >= 0; i--) {
        if (!my_std::is_in(ts[i], target)) break;
    }
    return i;
}

namespace htf {
    using namespace output;
    using namespace path_deal;
    using namespace stream;
    namespace core {    
        // *****************************************************************    
        /**
         * * 头文件依赖需要满足为 有向无环图
         * * 多个文件可能有多个 有向无环图
         * 设计：
         *     采用邻接表法
        */
        struct Node {
            Hpath hpath;
            Node* next;
            Node(const Hpath& s) :hpath{ s }, next{ nullptr } { }
            Node(const Hpath& s, Node* n) :hpath{ s }, next{ n } { }
        };

        class Graph {
        private:
            vector<pair<Node*, int>> _vertaxs;      // second->入度

            int _index(const Hpath& h) const
            {
                int i = _vertaxs.size() - 1;
                for (; i >= 0; i--)
                    if (_vertaxs[i].first->hpath == h) break;
                return i;
            }

            vector<Node*> _first() const
            {
                vector<Node*> res;
                for (const auto& t : _vertaxs) {
                    if (t.second == 0) res.emplace_back(t.first);
                }
                return res;
            }

            // 有相交的点则 unite，否则直接 emplace_back
            void _push(vector<vector<Hpath>>& ts, const vector<Hpath>& t) const
            {
                for (auto& x : ts) {
                    if (unite_reliance(x, t)) return;
                }
                ts.emplace_back(t);
            }

            // res 的节点个数是否与 _vertaxe 的相同
            bool _equal(const vector<vector<Hpath>>& ts) const
            {
                int cnt = 0;
                for (const auto& t : ts) cnt += t.size();
                return cnt == _vertaxs.size();
            }

            // 找出第一个不存在 (用于提示错误发生的文件)
            Hpath _first_not_find(const vector<vector<Hpath>>& ts) const
            {
                for (const auto& t : _vertaxs) {
                    for (const auto& target : ts) {
                        if (!my_std::is_in(t.first->hpath, target)) 
                            return t.first->hpath;
                    }
                }
                throw exception::Excep_dev{"preprocess.cpp::_first_not_find", _LINE + "所有节点都存在"};
                return _vertaxs[0].first->hpath;
            }

            // * 深度遍历：[注意] 邻接表 (返回连通点)
            void _dfs(vector<Hpath>& nodes, Node* cur) const
            {
                if (cur == nullptr || my_std::is_in(cur->hpath, nodes)) return;
                nodes.emplace_back(cur->hpath);
                while (cur->next) {
                    cur = cur->next;
                    _dfs(nodes, _vertaxs[_index(cur->hpath)].first);
                }
            }
            
            // 返回不同连通图的节点集合
            vector<vector<Hpath>> _differ() const
            {
                vector<vector<Hpath>> res;
                vector<Node*> firs = _first();
                if (firs.empty() && !_vertaxs.empty())  
                    throw exception::Excep_syntax{_vertaxs[0].first->hpath.str(), 0, "存在循环依赖"};
                for (Node* t : firs) {
                    vector<Hpath> temp;
                    _dfs(temp, t);
                    _push(res, temp);
                }
                if (!_equal(res)) 
                    throw exception::Excep_syntax{_first_not_find(res).str(), 0, "存在循环依赖"};
                return res;
            }

            // 对单个连通图进行拓扑排序，返回有序的结果
            vector<Hpath> _top_sort(const vector<Hpath>& nodes)
            {
                if (nodes.empty()) return {};
                vector<Hpath> res;
                stack<Hpath> stk;
                unordered_map<int, int> temp;
                for (const auto& t : nodes) 
                    temp[_index(t)] = _vertaxs[_index(t)].second;
                
                // 入度为0的入栈
                for (const auto& t : temp) {
                    if (t.second == 0) stk.push(_vertaxs[t.first].first->hpath);
                }
                while (!stk.empty()) {
                    Hpath top = stk.top();
                    res.emplace_back(top);
                    stk.pop();
                    // 将 top 指向的所有临边的入度减一
                    Node* n = _vertaxs[_index(top)].first->next;
                    while (n != nullptr) {
                        int i = _index(n->hpath);
                        temp[i]--;
                        if (temp[i] == 0) stk.push(n->hpath);    // 入度为0的入栈
                        n = n->next;
                    }
                }
                if (res.size() < nodes.size()) 
                    throw exception::Excep_syntax{nodes[index_first_not_find(nodes, res)].str(), 0, "存在循环依赖"};
                return res;
            }
            
        public:
            Graph(const Hpath& t)
                :_vertaxs{  }
            {
                Node* node = new Node{ t };
                _vertaxs.emplace_back(make_pair(node, 0));
            }
        
            Graph(const vector<Hpath>& t)
                :_vertaxs{  }
            {
                for (const auto& x : t) {
                    Node* node = new Node{ x };
                    if (_index(x) != -1) throw exception::Excep_dev{"preprocess.cpp::Graph::Graph",  _LINE +
                        "Node" + mark_string(x.str()) + "already exist"};
                    _vertaxs.emplace_back(make_pair(node, 0));
                }
            }

            ~Graph()
            {
                for (const auto& t : _vertaxs) {
                    Node* node = t.first;
                    while (node != nullptr) {
                        Node* temp = node;
                        node = node->next;
                        delete temp;
                    }
                }
            }
            // v -> n
            // * 当 v -> n 存在时返回 false (故采用尾插法)
            bool insert_edge(const Hpath& v, const Hpath& n)
            {
                int index1 = _index(v), index2 = _index(n);
                Node* new_node = new Node{n};
                if (index1 != -1) {
                    Node* node = _vertaxs[index1].first;
                    while (node->next) {
                        node = node->next;
                        if (node->hpath == n) return false;
                    }
                    node->next = new_node;
                } 
                else {
                    Node* temp = new Node{v, new_node};
                    _vertaxs.emplace_back(make_pair(temp, 0));
                }
                if (index2 == -1) {
                    _vertaxs.emplace_back(make_pair(new_node, 0));
                    index2 = _vertaxs.size() - 1;        // * update index
                }
                _vertaxs[index2].second++;      // * 入度加一
                return true;
            } 

            vector<vector<Hpath>> top_sorts()
            {
                vector<vector<Hpath>> temp = _differ();
                vector<vector<Hpath>> res;
                for (const auto& x : temp) 
                    res.emplace_back(_top_sort(x));
                return res;
            }
        };
        // ***************************************************************** 

        static vector<vector<pair<bool, Hpath>>> legal(const vector<vector<Hpath>>& ts, const vector<Hpath>& source)
        {
            vector<vector<pair<bool, Hpath>>> res;
            for (const auto& t : ts) {
                vector<pair<bool, Hpath>> temps;
                for (const auto& x : t) {
                    pair<bool, Hpath> temp{false, x};
                    if (my_std::is_in(x, source)) temp.first = true;
                    temps.emplace_back(temp);
                }
                res.emplace_back(temps);
            }
            return res;
        }
        

        static void preprocess_recur(Graph& v, const Hpath& source, const vector<Hdir>& include)
        {
            ifstream ifs{source.str()};
            stream::Token_stream ts{ifs};
            while (!ts.eof()) {
                ts.ignore(stream::Token{'#'});
                stream::Token token = ts.get();
                // #include "file.h"
                if (token.val == "include" && ts.peek().kind == stream::Token_kind::LITERAL_KIND && ts.peek().val[0] == '"') {
                    string file = ts.get().val;
                    file = file.substr(1, file.size() - 2);     // 不要 ""
                    bool found = false;
                    string path;
                    for (const auto& dir : include) {
                        if (dir.have_file(file)) {
                            found = true;
                            path = path_deal::normalize(dir.str(), file);
                            break;
                        }
                    }
                    if (!found) throw exception::Excep_syntax{source.str(), ts.line(), "not find" + mark_string(file)};
                    Hpath temp{ path };
                    if (temp == source)
                        throw exception::Excep_syntax{source.str(), ts.line(), "存在循环依赖"};
                    if (v.insert_edge(temp, source))
                        preprocess_recur(v, temp, include);
                }
            }
        }
        
        // ************************************************************************************************

        vector<vector<pair<bool, Hpath>>> preprocess(const vector<Hpath>& source, vector<Hdir> include)
        {
            Graph v(source);
            for (const auto& t : source) {
                if (!my_std::is_in(t.parent(), include)) 
                    include.emplace_back(t.parent());
            }
            for (const auto& t : source) {
                preprocess_recur(v, t, include);
            }
            vector<vector<Hpath>> tsort = v.top_sorts();
            vector<pair<Hpath, vector<Hpath>>> res;
            return legal(tsort, source);
        }
    }
}