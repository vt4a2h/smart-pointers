#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <iomanip>

#include "autoptr.h"
#include "scopedptr.h"
#include "sharedptr.h"

using namespace std;

class A {
public:
    A(int v = 0) : _v(v) {}
    ~A() { std::cout << "A deleted" << std::endl; }
    int get() const {return _v;}
    void set(int v) {_v = v;}

private:
    int _v;
};

std::unique_ptr<int> source(int i)
{
  return std::unique_ptr<int>(new int(i));
}

int main()
{
  std::unique_ptr<int> p = source(5);

  std::cout<< typeid(decltype(p)).name() << std::endl;
}

