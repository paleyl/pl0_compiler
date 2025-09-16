#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

class Base {
 public:
  virtual void f() { std::cout << "Base::f" << std::endl;}
  virtual void g() { std::cout << "Base::g" << std::endl;}
  virtual void h() { std::cout << "Base::h" << std::endl;}
  void m() {std::cout << "Base::m" << std::endl;}
 private:
  void n() {std::cout << "Base::n" << std::endl;}
  int a = 10;
  int b = 12;
};

int main(int argc, char** argv) {
  typedef void(*Fun)(void);
  //typedef void(Fun1)(void);
  Base b;
  Fun pFun = NULL;
  std::cout << "b location " << (&b) << std::endl;
  std::cout << "虚函数表地址：" << (int*)(&b) << std::endl;
  std::cout << "虚函数表 — 第1个函数指针地址：" << (int*)*(int*)(&b) << " 函数地址：" << (int*)*((int*)*(int*)(&b)) << std::endl;
  std::cout << "虚函数表 — 第2个函数指针地址：" << ((int*)*(int*)(&b) + 2) << " 函数地址：" << (int*)*((int*)*(int*)(&b) + 2) << std::endl;
  std::cout << "虚函数表 — 第3个函数指针地址：" << ((int*)*(int*)(&b) + 4) << " 函数地址：" << (int*)*((int*)*(int*)(&b) + 4) << std::endl;
  std::cout << "虚函数表 — 第3个函数指针地址：" << ((int*)*(int*)(&b) + 6) << " 函数地址：" << (int*)*((int*)*(int*)(&b) + 6) << std::endl;
  pFun = (Fun)*((int*)*(int*)(&b));
  pFun();
//  Fun pFun1 = NULL;
  pFun = (Fun)*((int*)*(int*)(&b) + 2);  // 双int字长
  pFun();
  pFun = (Fun)*((int*)*(int*)(&b) + 4);  // 双int字长
  pFun();
  // pFun = (Fun)*((int*)*(int*)(&b) + 2);  // 双int字长
  // pFun();
  // pFun = (Fun)*((int*)*(int*)(&b) + 6);  // 双int字长
  // pFun();
  int s = sizeof(Base) / sizeof(int);
  std::cout << "Base size = " << s << std::endl;
  // pFun = (Fun)*((int*)*(int*)(&b) + 6);  // 双int字长
  // pFun();
  // std::cout << (int*)(&(Base::m)) << std::endl;
  typedef void (Base::*MFun)();
  MFun pMFun = &Base::m;
  //(b.*pMFun)();
  char buf[] = {0};
  sprintf(buf, "%u", pMFun);
  int a = std::atoi(buf);
  std::cout << "M address " << (int*)(a) << std::endl;
  pFun = (Fun)(a);
  pFun();
  int mm = 1200;
  // pMFun = &Base::n;
  // sprintf(buf, "%u", pMFun);
  // a = std::atoi(buf);
  // std::cout << "N address " << (int*)a << std::endl;
  std::cout << ((int*)(&b) + 1) << " " << *((int*)(&b) + 1) << std::endl; 
  std::cout << ((int*)(&b) + 2) << " " << *((int*)(&b) + 2) << std::endl;  // 成员变量
  std::cout << ((int*)(&b) + 3) << " " << *((int*)(&b) + 3) << std::endl;  // 成员变量
  std::cout << ((int*)(&b) + 4) << " " << (int*)*((int*)(&b) + 4) << std::endl;
  std::cout << ((int*)(&b) + 5) << " " << (int*)*((int*)(&b) + 5) << std::endl;
  std::cout << ((int*)(&b) + 6) << " " << (int*)*((int*)(&b) + 6) << std::endl;
  std::cout << ((int*)(&b) + 7) << " " << (int*)*((int*)(&b) + 7) << std::endl;
  std::cout << ((int*)(&b) + 8) << " " << (int*)*((int*)(&b) + 8) << std::endl;
  std::cout << ((int*)(&b) + 9) << " " << (int*)*((int*)(&b) + 9) << std::endl;
  std::cout << ((int*)(&b) + 10) << " " << (int*)*((int*)(&b) + 10) << std::endl;
  pFun = (Fun)*((int*)(&b) + 6);  // 成员函数指针
  std::cout << (int*)*((int*)(&b) + 6) << std::endl;
  std::cout << (int*)*((int*)*(int*)(&b) + 4) << std::endl;
  // pFun();
  // b.m();
  // pFun = (Fun)*((int*)(&b) + 8);  // 成员函数指针
  // pFun();
  //b.n();
}
