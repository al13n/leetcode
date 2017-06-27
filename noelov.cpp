// Order Book: Voleon
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <cfloat>

#define UL unsigned long int
using namespace std;

enum class Type {
    Market,
    Limit, 
    Stop,
    Cancel
};
    
enum class Side {
    Buy,
    Sell,
    NA
};

enum class HasThresholdPrice {
    False,
    True
};

struct Order {
    Type type;
    Side side;
    UL val1;
    double val2;
    static UL count;
    UL index;
    
    shared_ptr<Order> previous;
    shared_ptr<Order> next;
    
    Order() {}

    Order(const string &t, const string &s, const UL &v1, const double &v2): val1{v1}, val2{v2}, previous{nullptr}, next{nullptr} {
        index = ++count;
        
        /*set type of order*/
        if (t == "market") {
            type = Type::Market;
        }
        else if (t == "limit") {
            type = Type::Limit;    
        }
        else if (t == "stop") {
            type = Type::Stop;
        }
        else {
            type = Type::Cancel;
        }
        
        /*set side of order*/
        if (s == "buy") {
            side = Side::Buy;
        }
        else if (s == "sell") {
            side = Side::Sell;
        }
        else {
            side = Side::NA;
        }
    }
};

UL Order::count = 0;

struct sell_options_comparator {
  bool operator() (const shared_ptr<Order> &o1, const shared_ptr<Order> &o2) const{
      if (o1->val2 == o2->val2) {
          return o1->index < o2->index;
      }
      return o1->val2 < o2->val2;
  }  
};

struct buy_options_comparator {
  bool operator() (const shared_ptr<Order> &o1, const shared_ptr<Order> &o2) const{
      if (o1->val2 == o2->val2) {
          return o1->index < o2->index;
      }
      return o1->val2 > o2->val2;
  }  
};

set< shared_ptr<Order>, sell_options_comparator> sell_options;
set< shared_ptr<Order>, buy_options_comparator> buy_options;

shared_ptr<Order> current_order = nullptr;
shared_ptr<Order> head_stoporder_list = nullptr;
shared_ptr<Order> tail_stoporder_list = nullptr;

unordered_map< UL, shared_ptr<Order> > index_ptr;

double max_trade_val2 = DBL_MIN;
double min_trade_val2 = DBL_MAX;
bool trade_made = false;

void removeOrderFromMap(const UL &index) {
    if (index_ptr.find(index) != index_ptr.end()){
        index_ptr.erase(index_ptr.find(index));
    }
}

void removeOrderFromDataList(shared_ptr<Order> &head, shared_ptr<Order> &tail, shared_ptr<Order> &o) {
    if (tail == o && tail != nullptr) {
        tail = tail->next;
    }    
    if(head == o && head != nullptr) {
        head = head->previous;
    } 
    if(o != nullptr) {
        if (o->previous != nullptr) {
            o->previous->next = o->next;
        }   
        if (o->next != nullptr) {
            o->next->previous = o->previous;
        }
        o->next = o->previous = nullptr;
    }
}
    
void addOrderToMap(shared_ptr<Order> &o) {
    if (index_ptr.find(o->index) == index_ptr.end()) {
        index_ptr.insert(make_pair(o->index, o));
    }
}

void addOrderToDataList(shared_ptr<Order> &head, shared_ptr<Order> &tail, shared_ptr<Order> &o) {
    if (head!= nullptr) {
        head->next = o;
        o->previous = head;
        head = o;
    } else {
        head = o;
        tail = o;
    }
    addOrderToMap(o);
}

void createNewOrder(const string &type, const string &side, const UL &val1, const double &val2) {
    auto new_order = make_shared<Order>(type, side, val1, val2);
    switch(new_order->type) {
        case Type::Stop :
            addOrderToDataList(head_stoporder_list, tail_stoporder_list, new_order);
            addOrderToMap(new_order);
            break;
        default :
            break;
    }
    current_order = new_order;
}

void match(shared_ptr<Order> &o1, shared_ptr<Order> &o2) {
    UL trade_volume = min(o1->val1, o2->val1);
    printf("match %lu %lu %lu %.2f\n", o1->index, o2->index, trade_volume, o2->val2);
    o1->val1 = o1->val1 - trade_volume;
    o2->val1 = o2->val1 - trade_volume;
    max_trade_val2 = max(max_trade_val2, o2->val2);
    min_trade_val2 = min(min_trade_val2, o2->val2);
    trade_made = true;
}

void makeBuyTrade(shared_ptr<Order> &o, HasThresholdPrice has_threshold) {
    while (!sell_options.empty() && o->val1 > 0) {
        shared_ptr<Order> match_order = *(sell_options.begin());
        if (has_threshold == HasThresholdPrice::True && o->val2 < match_order->val2) {
            break;
        }  
        match(o, match_order);
        if (match_order->val1 == 0) {
            removeOrderFromMap(match_order->index);
            sell_options.erase(sell_options.begin());
        }
    }
}

void makeSellTrade(shared_ptr<Order> &o, HasThresholdPrice has_threshold) {
    while (!buy_options.empty() && o->val1 > 0) {
        shared_ptr<Order> match_order = *(buy_options.begin());
        if (has_threshold == HasThresholdPrice::True && o->val2 > match_order->val2) {
            break;
        }
        match(o, match_order);
        if (match_order->val1 == 0) {
            removeOrderFromMap(match_order->index);
            buy_options.erase(buy_options.begin());
        }
    }  
}

void executeOrder() {
    switch (current_order->type) {
        case Type::Market :   
            switch (current_order->side) {
                case Side::Buy :
                    makeBuyTrade(current_order, HasThresholdPrice::False);
                    break;
                case Side::Sell :
                    makeSellTrade(current_order, HasThresholdPrice::False);
                    break;
            }
            break;
        case Type::Limit  :
            switch (current_order->side) {
                case Side::Buy :
                    makeBuyTrade(current_order, HasThresholdPrice::True);
                    break;
                case Side::Sell :
                    makeSellTrade(current_order, HasThresholdPrice::True);
                    break;
                default:
                    break;
            }
            if (current_order->val1 != 0) {
                switch(current_order->side) {
                    case Side::Buy :
                        buy_options.insert(current_order);
                        addOrderToMap(current_order);
                        break;
                    case Side::Sell :
                        sell_options.insert(current_order);
                        addOrderToMap(current_order);
                        break;
                    default:
                        break;
                } 
            }
            break;
        case Type::Stop   :
            break;
        case Type::Cancel :
            if (index_ptr.find(current_order->val1) != index_ptr.end()) {
                auto order_element = index_ptr[current_order->val1];
                switch (order_element->type) {
                    case Type::Limit:
                        switch(order_element->side) {
                            case Side::Buy :
                                buy_options.erase(buy_options.find(order_element));
                                break;
                            case Side::Sell :
                                sell_options.erase(sell_options.find(order_element));
                                break;
                            default:
                                break;
                        }
                        removeOrderFromMap(order_element->index);
                        break;
                    case Type::Stop:
                        removeOrderFromDataList(head_stoporder_list, tail_stoporder_list, order_element);
                        removeOrderFromMap(order_element->index);
                        break;
                    default:
                        break;
                }
            }
            break;
    }
}

void checkAndTriggerStopOrders() {
    bool check_for_trigger = true;
    while (trade_made && check_for_trigger) {
        bool executed = false;
        for (auto itr = tail_stoporder_list; itr != nullptr && !executed; itr = itr->next) {
            switch (itr->side) {
                case Side::Buy :
                    if (itr->val2 <=  max_trade_val2 && trade_made) {
                        executed = true;
                        makeBuyTrade(itr, HasThresholdPrice::False);
                    }
                    break;
                case Side::Sell :
                    if (itr->val2 >= min_trade_val2 && trade_made) {
                        executed = true;
                        makeSellTrade(itr, HasThresholdPrice::False);
                    }
                    break;
                default:
                    break;
            }
            if (executed) {
                removeOrderFromDataList(head_stoporder_list, tail_stoporder_list, itr);
                removeOrderFromMap(itr->index);
            }
        }
        check_for_trigger = executed;
    }
   
    max_trade_val2 = DBL_MIN;
    min_trade_val2 = DBL_MAX;
    trade_made = false;
}

int main() {
    string input;
    while(getline(cin, input)) {
        stringstream ss(input);
        string side, type;
        UL val1;
        double val2;
        
        ss >> type >> side >> val1 >> val2;
        createNewOrder(type, side, val1, val2);
        executeOrder();
        checkAndTriggerStopOrders();
    }
    
    return 0;
}
