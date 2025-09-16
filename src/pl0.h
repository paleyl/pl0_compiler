
typedef enum {
  false,
  true
} bool;

#define norw 13  // 关键字个数
#define txmax 100
#define nmax 14
#define al 10
#define amax 2047
#define levmax 3
#define cxmax 200

enum symbol {
  nul, ident, number, plus, minus, 
  times, slash, oddsym, eql, neq,
  lss, leq, gtr, geq, lparen,
  rparen, comma, semicolon, period, becomes,
  beginsym, endsym, ifsym, thensym, whilesym,
  writesym, readsym, dosym, callsym, constsym,
  varsym, procsym,
};

#define symnum 32

enum object {
  constant,
  variable,
  procedur,
};

enum fct {
  lit, opr, lod,
  sto, cal, inte,
  jmp, jpc,
};
#define fctnum 8

struct instruction {
  enum fct f;
  int l;
  int a;
};
FILE* fas;
FILE* fa;
FILE* fa1;
FILE* fa2;
FILE* fstack;
bool listswitch;
bool tableswitch;
char ch;
enum symbol sym;
char id[al+1];
int num;
int cc, ll;
int cx;
char line[81];
char a[al+1];
struct instruction code[cxmax];
char word[norw][al];  // 保留字，用于程序解析
enum symbol wsym[norw];  // 保留字对应的符号值
enum symbol ssym[256];  // 单字符对应的符号值
char mnemonic[fctnum][5];  // 虚拟机代码指令名称，用作指令打印
bool declbegsys[symnum];  // 声明开始的符号集合 ("const", "var", "procedure")
bool statbegsys[symnum];  // 语句开始的符号集合 ("begin", "call", "if", "while")
bool facbegsys[symnum];  // 因子开始的符号集合 (ident, number, "(")

struct tablestruct
{
  char name[al];
  enum object kind;
  int val;
  int level;
  int adr;
  int size;
};

struct tablestruct table[txmax];
FILE* fin;
FILE* fout;
char fname[al];
int err;

#define getsymdo if(-1 == getsym()) return -1
#define getchdo if(-1 == getch()) return -1
#define testdo(a,b,c) if(-1 == test(a,b,c)) return -1
#define gendo(a,b,c) if (-1 == gen(a,b,c)) return -1
#define expressiondo(a,b,c) if(-1 == expression(a,b,c)) return -1
#define factordo(a,b,c) if(-1 == factor(a,b,c)) return -1
#define termdo(a,b,c) if(-1 == term(a,b,c)) return -1
#define conditiondo(a,b,c) if(-1 == condition(a,b,c)) return -1
#define statementdo(a,b,c) if(-1 == statement(a,b,c)) return -1
#define constdeclarationdo(a,b,c) if(-1 == constdeclaration(a,b,c)) return -1
#define vardeclarationdo(a,b,c) if(-1 == vardeclaration(a,b,c)) return -1
void error1(int n, int line, char* file);
int getsym();
int getch();
void init();
int gen(enum fct x, int y, int z);
int test(bool* sl, bool* s2, int n);
int inset(int e, bool* s);
int addset(bool* sr, bool* s1, bool* s2, int n);
int subset(bool* sr, bool* s1, bool* s2, int n);
int mulset(bool* sr, bool* s1, bool* s2, int n);
int block(int lev, int tx, bool* fsys);
void interpret();
int factor(bool* fsys, int* ptx, int lev);
int term(bool* fsys, int* ptx, int lev);
int condition(bool* fsys, int* ptx, int lev);
int expression(bool* fsys, int* ptx, int lev);
int statement(bool* fsys, int* ptx, int lev);
void listcode(int cx0, FILE* fout);
int vardeclaration(int* ptx, int lev, int* pdx);
int constdeclaration(int* ptx, int lev, int* pdx);
int position(char* idt, int tx);
void enter(enum object k, int* ptx, int lev, int* pdx);
int base(int l, int* s, int b);
