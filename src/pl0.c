#include <stdio.h>
#include "string.h"
#include "pl0.h"

#define stacksize 500

int compile(char* infile) {
  bool nxtlev[symnum];
  listswitch = true;
  tableswitch = true;
  fin = fopen("infile1.txt", "r");
  fa1 = fopen("fa1.tmp", "w");
  init();
  err = 0;
  cc = cx = ll = 0;
  ch = ' ';
  if (-1 != getsym()) {
    // printf("get symbol not -1\n");
    fa = fopen("fa.tmp", "w");
    fas = fopen("fas.tmp", "w");
    addset(nxtlev, declbegsys, statbegsys, symnum);
    nxtlev[period] = true;
    if (-1 == block(0, 0, nxtlev)) {
      fclose(fa);
      fclose(fa1);
      fclose(fas);
      fclose(fin);
      printf("\n");
      return 0;
    }
    fclose(fa);
    fclose(fa1);
    fclose(fas);
    if (sym != period) {
      error(9);
    }
    if (err == 0) {
      fa2 = fopen("fa2.tmp", "w");
      interpret();
      fclose(fa2);
    } else {
      printf("Errors in pl/0 program");
    }
    fclose(fin);
  }
  return 0;
}

void init() {
  int i;
  for (i = 0; i <= 255; ++i) {
    ssym[i] = nul;
  }
  ssym['+'] = plus;
  ssym['-'] = minus;
  ssym['*'] = times;
  ssym['/'] = slash;
  ssym['('] = lparen;
  ssym[')'] = rparen;
  ssym['='] = eql;
  ssym[','] = comma;
  ssym['.'] = period;
  ssym['#'] = neq;
  ssym[';'] = semicolon;

  strcpy(&(word[0][0]), "begin");
  strcpy(&(word[1][0]), "call");
  strcpy(&(word[2][0]), "const");
  strcpy(&(word[3][0]), "do");
  strcpy(&(word[4][0]), "end");
  strcpy(&(word[5][0]), "if");
  strcpy(&(word[6][0]), "odd");
  strcpy(&(word[7][0]), "procedure");
  strcpy(&(word[8][0]), "read");
  strcpy(&(word[9][0]), "then");
  strcpy(&(word[10][0]), "var");
  strcpy(&(word[11][0]), "while");
  strcpy(&(word[12][0]), "write");

  wsym[0] = beginsym;
  wsym[1] = callsym;
  wsym[2] = constsym;
  wsym[3] = dosym;
  wsym[4] = endsym;
  wsym[5] = ifsym;
  wsym[6] = oddsym;
  wsym[7] = procsym;
  wsym[8] = readsym;
  wsym[9] = thensym;
  wsym[10] = varsym;
  wsym[11] = whilesym;
  wsym[12] = writesym;

  strcpy(&(mnemonic[lit][0]), "lit");
  strcpy(&(mnemonic[opr][0]), "opr");
  strcpy(&(mnemonic[lod][0]), "lod");
  strcpy(&(mnemonic[sto][0]), "sto");
  strcpy(&(mnemonic[cal][0]), "cal");
  strcpy(&(mnemonic[inte][0]), "int");
  strcpy(&(mnemonic[jmp][0]), "jmp");
  strcpy(&(mnemonic[jpc][0]), "jpc");

  for (i = 0; i < symnum; ++i) {
    declbegsys[i] = false;
    statbegsys[i] = false;
    facbegsys[i] = false;
  }
  declbegsys[constsym] = true;  // 声明语句开始符号集合 ("const", "var", "procedure")
  declbegsys[varsym] = true;
  declbegsys[procsym] = true;

  statbegsys[beginsym] = true;  // 语句开始的符号集合 ("begin", "call", "if", "while")
  statbegsys[callsym] = true;
  statbegsys[ifsym] = true;
  statbegsys[whilesym] = true;

  facbegsys[ident] = true;  // 因子开始的符号集合 (ident, number, "(")
  facbegsys[number] = true;
  facbegsys[lparen] = true;
}

int inset(int e, bool* s) {
  return s[e];
}
int addset(bool* sr, bool* s1, bool* s2, int n) {
  int i;
  for (i = 0; i < n; ++i) {
    sr[i] = s1[i] || s2[i];
  }
  return 0;
}
int subset(bool* sr, bool* s1, bool* s2, int n) {
  int i;
  for (i = 0; i < n; ++i) {
    sr[i] = s1[i] && (!s2[i]);
  }
  return 0;
}
int mulset(bool* sr, bool* s1, bool* s2, int n) {
  int i;
  for (i = 0; i < n; ++i) {
    sr[i] = s1[i] && s2[i];
  }
  return 0;
}

void error1(int n, int line, char* file) {
  char space[81];
  memset(space, 32, 81);
  space[cc - 1] = 0;
  printf("****%s error %d at [%s] %d\n", space, n, file, line);
  fprintf(fa1, "****%s error %d at [%s] %d\n", space, n, file, line);
  err++;
}

# define error(n) error1(n, __LINE__, __FILE__) 

int getch() {
  if (cc == ll) {
    if (feof(fin)) {
      printf("program incomplete");
      return -1;
    }
    ll = 0;
    cc = 0;
    printf("%d", cx);
    fprintf(fa1, "%d", cx);
    ch = ' ';
    while (ch != 10) {
      if (EOF == fscanf(fin, "%c", &ch)) {
        line[ll] = 0;
        break;
      }
      printf("%c", ch);
      fprintf(fa1, "%c", ch);
      line[ll] = ch;
      ll++;
    }
    printf("\n");
    fprintf(fa1, "\n");
  }
  ch = line[cc];
  cc++;
  return 0;
}

int getsym() {
  int i, j, k;
  while (ch == ' ' || ch == 10 || ch == 9) {
    getchdo;
  }
  if (ch >= 'a' && ch <= 'z') {
    k = 0;
    do {
      if (k < al) {
        a[k] = ch;
        ++k;
      }
      getchdo;
    } while ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'));
    a[k] = 0;
    strcpy(id, a);
    i = 0;
    j = norw - 1;
    do {
      k = (i + j) / 2;
      if (strcmp(id, word[k]) <= 0) {
        j = k - 1;
      }
      if (strcmp(id, word[k]) >= 0) {
        i = k + 1;
      }
    } while (i <= j);
    if (i - 1 > j) {
      sym = wsym[k];
    } else {
      sym = ident;
    }
  } else {
    if (ch >= '0' && ch <= '9') {
      k = 0;
      num = 0;
      sym = number;
      do {
        num = 10 * num + ch - '0';
        k++;
        getchdo;
      } while (ch >= '0' && ch <= '9');
      k--;
      if (k > nmax) {
        error(30);
      }
    } else {
      if (ch == ':') {
        getchdo;
        if (ch == '=') {
          sym = becomes;
          getchdo;
        } else {
          sym = nul;
        }
      } else {
        if (ch == '<') {
          getchdo;
          if (ch == '=') {
            sym = leq;
            getchdo;
          } else {
            sym = lss;
          }
        } else {
          if (ch == '>') {
            getchdo;
            if (ch == '=') {
              sym = geq;
              getchdo;
            } else {
              sym = gtr;
            }
          } else {
            sym = ssym[ch];
            if (sym != period) {
              getchdo;
            }
          }
        }
      }
    }
  }
  return 0;
}

int gen(enum fct x, int y, int z) {
  if (cx >= cxmax) {
    printf("Program too long\n");
    return -1;
  }
  code[cx].f = x;
  code[cx].l = y;
  code[cx].a = z;
  cx++;
  return 0;
}

int test(bool* s1, bool* s2, int n) {
  if (! inset(sym, s1)) {
    error(n);
    while ((! inset(sym, s1)) && (! inset(sym, s2))) {
      getsymdo;
    }
  }
  return 0;
}

#define COMPLETE1
#ifdef COMPLETE1
int block(int lev, int tx, bool* fsys) {
  int i;
  int dx;
  int tx0;
  int cx0;
  bool nxtlev[symnum];
  dx = 3;
  tx0 = tx;
  table[tx].adr = cx;
  gendo(jmp, 0, 0);
  if (lev > levmax) {
    error(32);
  }
  do {
    if (sym == constsym) {
      getsymdo;
      do {
        constdeclarationdo(&tx, lev, &dx);
        while (sym == comma) {
          getsymdo;
          constdeclarationdo(&tx, lev, &dx);
        }
        if (sym == semicolon) {
          getsymdo;
        } else {
          error(5);
        }
      } while (sym == ident);
    }
    if (sym == varsym) {
      getsymdo;
      do {
        vardeclarationdo(&tx, lev, &dx);
        while (sym == comma) {
          getsymdo;
          vardeclarationdo(&tx, lev, &dx);
        }
        if (sym == semicolon) {
          getsymdo;
        } else {
          error(5);
        }
      } while (sym == ident);
    }
    while (sym == procsym) {
      getsymdo;
      if (sym == ident) {
        enter(procedur, &tx, lev, &dx);
        getsymdo;
      } else {
        error(4);
      }
      if (sym == semicolon) {
        getsymdo;
      } else {
        error(5);
      }
      memcpy(nxtlev, fsys, sizeof(bool) * symnum);
      nxtlev[semicolon] = true;
      if (-1 == block(lev + 1, tx, nxtlev)) {
        return -1;
      }
      if (sym == semicolon) {
        getsymdo;
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        nxtlev[procsym] = true;
        testdo(nxtlev, fsys, 6);
      } else {
        error(5);
      }
    }
    memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
    nxtlev[ident] = true;
    nxtlev[period] = true;
    testdo(nxtlev, declbegsys, 7);
  } while (inset(sym, declbegsys));
  code[table[tx0].adr].a = cx;
  table[tx0].adr = cx;
  table[tx0].size = dx;
  cx0 = cx;
  gendo(inte, 0, dx);

  if (tableswitch) {
    printf("TABLE: \n");
    if (tx0 + 1 > tx) {
      printf("NULL\n");
    }
    for (i = tx0 + 1; i <= tx; i ++) {
      switch (table[i].kind)
      {
      case constant:
        printf("%d const %s, ", i, table[i].name);
        printf("val = %d\n", table[i].val);
        fprintf(fas, "%d const %s, ", i, table[i].name);
        fprintf(fas, "val = %d\n", table[i].val);
        break;
      case variable:
        printf("%d var %s, ", i, table[i].name);
        printf("lev = %d addr = %d\n", table[i].level, table[i].adr);
        fprintf(fas, "%d var %s, ", i, table[i].name);
        fprintf(fas, "lev = %d addr = %d\n", table[i].level, table[i].adr);
        break;
      case procedur:
        printf("%d proc %s, ", i, table[i].name);
        printf("lev = %d addr = %d size = %d\n", table[i].level, table[i].adr, table[i].size);
        fprintf(fas, "%d proc %s, ", i, table[i].name);
        fprintf(fas, "lev = %d addr = %d size = %d\n", table[i].level, table[i].adr, table[i].size);
      default:
        break;
      }
    }
    printf("\n");
  }
  memcpy(nxtlev, fsys, sizeof(bool) * symnum);
  nxtlev[semicolon] = true;
  nxtlev[endsym] = true;
  statementdo(nxtlev, &tx, lev);
  gendo(opr, 0, 0);
  memset(nxtlev, 0, sizeof(bool) * symnum);
  testdo(fsys, nxtlev, 3);
  listcode(cx0);
  return 0;
}
#endif  // COMPLETE1

void enter(enum object k, int* ptx, int lev, int* pdx) {
  (*ptx)++;
  strcpy(table[(*ptx)].name, id);
  table[(*ptx)].kind = k;
  switch (k) {
    case constant:
      if (num > amax) {
        error(31);
        num = 0;
      }
      table[(*ptx)].val = num;
      break;
    case variable:
      table[(*ptx)].level = lev;
      table[(*ptx)].adr = (*pdx);
      (*pdx)++;
      break;
    case procedur:
      table[(*ptx)].level = lev;
      break;
  }
}

int position(char* idt, int tx) {
  int i;
  strcpy(table[0].name, idt);
  i = tx;
  while (strcmp(table[i].name, idt) != 0) {
    i--;
  }
  return i;
}

int constdeclaration(int* ptx, int lev, int* pdx) {
  if (sym == ident) {
    getsymdo;
    if (sym == eql || sym == becomes) {
      if (sym == becomes) {
        error(1);
      }
      getsymdo;
      if (sym == number) {
        enter(constant, ptx, lev, pdx);
        getsymdo;
      } else {
        error(2);
      }
    } else {
      error(3);
    }
  } else {
    error(4);
  }
  return 0;
}

int vardeclaration(int* ptx, int lev, int* pdx) {
  if (sym == ident) {
    enter(variable, ptx, lev, pdx);
    getsymdo;
  } else {
    error(4);
  }
  return 0;
}

void listcode(int cx0) {
  int i;
  if (listswitch) {
    for (i = cx0; i < cx; i++) {
      printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
      fprintf(fa, "%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
    }
  }
}

int statement(bool* fsys, int* ptx, int lev) {
  int i, cx1, cx2;
  bool nxtlev[symnum];
  if (sym == ident) {
    i = position(id, *ptx);
    if (i == 0) {
      error(11);
    } else {
      if (table[i].kind != variable) {
        error(12);
        i = 0;
      } else {
        getsymdo;
        if (sym == becomes) {
          getsymdo;
        } else {
          error(13);
        }
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        expressiondo(nxtlev, ptx, lev);
        if (i != 0) {
          gendo(sto, lev - table[i].level, table[i].adr);
        }
      }
    }  // if (i == 0)
  } else {
    if (sym == readsym) {
      getsymdo;
      if (sym != lparen) {
        error(34);
      } else {
        do {
          getsymdo;
          if (sym == ident) {
            i = position(id, *ptx);
          } else {
            i = 0;
          }
          if (i == 0) {
            error(35);
          } else {
            gendo(opr, 0, 16);
            gendo(sto, lev - table[i].level, table[i].adr);
          }
          getsymdo;
        } while(sym == comma);
      }
      if (sym != rparen) {
        error(33);
        while(! inset(sym, fsys)) {
          getsymdo;
        }
      } else {
        getsymdo;
      }
    } else {
      if (sym == writesym) {
        getsymdo;
        if (sym == lparen) {
          do {
            getsymdo;
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[rparen] = true;
            nxtlev[comma] = true;
            expressiondo(nxtlev, ptx, lev);
            gendo(opr, 0, 14);
          } while(sym == comma);
          if (sym != rparen) {
            error(33);
          } else {
            getsymdo;
          }
        }
        gendo(opr, 0, 15);
      } else {
        if (sym == callsym) {
          getsymdo;
          if (sym != ident) {
            error(14);
          } else {
            i = position(id, *ptx);
            if (i == 0) {
              error(11);
            } else {
              if (table[i].kind == procedur) {
                gendo(cal, lev - table[i].level, table[i].adr);
              } else {
                error(15);
              }
            }
            getsymdo;
          }
        } else {
          if (sym == ifsym) {
            getsymdo;
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[thensym] = true;
            nxtlev[dosym] = true;
            conditiondo(nxtlev, ptx, lev);
            if (sym == thensym) {
              getsymdo;
            } else {
              error(16);
            }
            cx1 = cx;
            gendo(jpc, 0, 0);
            statementdo(fsys, ptx, lev);
            code[cx1].a = cx;
          } else {
            if (sym == beginsym) {
              getsymdo;
              memcpy(nxtlev, fsys, sizeof(bool) * symnum);
              nxtlev[semicolon] = true;
              nxtlev[endsym] = true;
              statementdo(nxtlev, ptx, lev);
              while (inset(sym, statbegsys) || sym == semicolon) {
                if (sym == semicolon) {
                  getsymdo;
                } else {
                  error(10);
                }
                statementdo(nxtlev, ptx, lev);
              }
              if (sym == endsym) {
                getsymdo;
              } else {
                error(17);
              }
            } else {
              if (sym == whilesym) {
                cx1 = cx;
                getsymdo;
                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                nxtlev[dosym] = true;
                conditiondo(nxtlev, ptx, lev);
                cx2 = cx;
                gendo(jpc, 0, 0);
                if (sym == dosym) {
                  getsymdo;
                } else {
                  error(18);
                }
                statementdo(fsys, ptx, lev);
                gendo(jmp, 0, cx1);
                code[cx2].a = cx;
              } else {
                memset(nxtlev, 0, sizeof(bool) * symnum);
                testdo(fsys, nxtlev, 19);
              }
            }
          }
        }
      }
    } 
  } 
  return 0;
}

int expression(bool* fsys, int* ptx, int lev) {
  enum symbol addop;
  bool nxtlev[symnum];
  if (sym == plus || sym == minus) {
    addop = sym;
    getsymdo;
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[plus] = true;
    nxtlev[minus] = true;
    termdo(nxtlev, ptx, lev);
    if (addop == minus) {
      gendo(opr, 0, 1);
    }
  } else {
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[plus] = true;
    nxtlev[minus] = true;
    termdo(nxtlev, ptx, lev);
  }
  while (sym == plus || sym == minus) {
    addop = sym;
    getsymdo;
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[plus] = true;
    nxtlev[minus] = true;
    termdo(nxtlev, ptx, lev);
    if (addop == plus) {
      gendo(opr, 0, 2);
    } else {
      gendo(opr, 0, 3);
    }
  }
  return 0;
}

int term(bool* fsys, int* ptx, int lev) {
  enum symbol mulop;
  bool nxtlev[symnum];
  memcpy(nxtlev, fsys, sizeof(bool) * symnum);
  nxtlev[times] = true;
  nxtlev[slash] = true;
  factordo(nxtlev, ptx, lev);
  while (sym == times || sym == slash) {
    mulop = sym;
    getsymdo;
    factordo(nxtlev, ptx, lev);
    if (mulop == times) {
      gendo(opr, 0, 4);
    } else {
      gendo(opr, 0, 5);
    }
  }
  return 0;
}

int factor(bool* fsys, int* ptx, int lev) {
  int i;
  bool nxtlev[symnum];
  testdo(facbegsys, fsys, 24);
  while (inset(sym, facbegsys)) {
    if (sym == ident) {
      i = position(id, *ptx);
      if (i == 0) {
        error(11);
      } else {
        switch (table[i].kind) {
          case constant:
            gendo(lit, 0, table[i].val);
            break;
          case variable:
            gendo(lod, lev - table[i].level, table[i].adr);
            break;
          case procedur:
            error(21);
            break;
        }
      }
      getsymdo;
    } else {
      if (sym == number) {
        if (num > amax) {
          error(31);
          num = 0;
        }
        gendo(lit, 0, num);
        getsymdo;
      } else {
        if (sym == lparen) {
          getsymdo;
          memcpy(nxtlev, fsys, sizeof(bool) * symnum);
          nxtlev[rparen] = true;
          expressiondo(nxtlev, ptx, lev);
          if (sym == rparen) {
            getsymdo;
          } else {
            error(22);
          }
        }
        testdo(fsys, facbegsys, 23);
      }
    }
  }
  return 0;
}

int condition(bool* fsys, int* ptx, int lev) {
  enum symbol relop;
  bool nxtlev[symnum];
  if (sym == oddsym) {
    getsymdo;
    expressiondo(fsys, ptx, lev);
    gendo(opr, 0, 6);
  } else {
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[eql] = true;
    nxtlev[neq] = true;
    nxtlev[lss] = true;
    nxtlev[leq] = true;
    nxtlev[gtr] = true;
    nxtlev[geq] = true;
    expressiondo(nxtlev, ptx, lev);
    if (sym != eql && sym != neq && sym != lss && sym != neq && sym != gtr && sym != geq) {
      error(20);
    } else {
      relop = sym;
      getsymdo;
      expressiondo(fsys, ptx, lev);
      switch(relop) {
        case eql:
          gendo(opr, 0, 8);
          break;
        case neq:
          gendo(opr, 0, 9);
          break;
        case lss:
          gendo(opr, 0, 10);
          break;
        case geq:
          gendo(opr, 0, 11);
          break;
        case gtr:
          gendo(opr, 0, 12);
          break;
        case leq:
          gendo(opr, 0, 13);
          break;
      }
    }
  }
  return 0;
}

int base(int l, int* s, int b) {
  int b1;
  b1 = b;
  while (l > 0) {
    b1 = s[b1];
    l--;
  }
  return b1;
}

void interpret() {
  int p, b, t;
  struct instruction i;
  int s[stacksize];
  printf("start pl0\n");
  t = 0;
  b = 0;
  p = 0;
  s[0] = s[1] = s[2] = 0;
  do {
    i = code[p];
    p++;
    switch (i.f) {
      case lit:
        s[t] = i.a;
        t++;
        break;
      case opr:
        switch (i.a) {
          case 0:
            t = b;
            p = s[t + 2];
            b = s[t + 1];
            break;
          case 1:
            s[t - 1] = -s[t - 1];
            break;
          case 2:
            t--;
            s[t - 1] = s[t - 1] + s[t];
            break;
          case 3:
            t--;
            s[t - 1] = s[t - 1] - s[t];
            break;
          case 4:
            t--;
            s[t - 1] = s[t - 1] * s[t];
            break;
          case 5:
            t--;
            s[t - 1] = s[t - 1] / s[t];
            break;
          case 6:
            s[t - 1] = s[t - 1] % 2;
            break;
          case 8:
            t--;
            s[t - 1] = (s[t - 1] == s[t]);
            break;
          case 9:
            t--;
            s[t - 1] = (s[t - 1] != s[t]);
            break;
          case 10:
            t--;
            s[t - 1] = (s[t - 1] < s[t]);
            break;
          case 11:
            t--;
            s[t - 1] = (s[t - 1] >= s[t]);
            break;
          case 12:
            t--;
            s[t - 1] = (s[t - 1] > s[t]);
            break;
          case 13:
            t--;
            s[t - 1] = (s[t - 1] <= s[t]);
            break;
          case 14:
            printf("%d", s[t - 1]);
            fprintf(fa2, "%d", s[t - 1]);
            t--;
            break;
          case 15:
            printf("\n");
            fprintf(fa2, "\n");
            break;
          case 16:
            printf("?");
            fprintf(fa2, "?");
            scanf("%d", &s[t]);
            fprintf(fa2, "%d\n", s[t]);
            t++;
            break;
        }
        break;
      case lod:
        s[t] = s[base(i.l, s, b) + i.a];
        t++;
        break;
      case sto:
        t--;
        s[base(i.l, s, b) + i.a] = s[t];
        break;
      case cal:
        s[t] = base(i.l, s, b);
        s[t + 1] = b;
        s[t + 2] = p;
        b = t;
        p = i.a;
        break;
      case inte:
        t += i.a;
        break;
      case jmp:
        p = i.a;
        break;
      case jpc:
        t--;
        if (s[t] == 0) {
          p = i.a;
        }
        break;
    }
  } while(p != 0);
}

/*
* A function for test
* */
void trials(int argc, char** argv) {
  bool alei = true;
  printf("hello compiler: %d\n", alei);
  bool a[3] = {true, true, false};
  bool b[3] = {false, true, true};
  bool c[3];
  subset(c, a, b, 3);
  int i;
  for (i = 0; i < 3; ++i) {
    printf("%d ", c[i]);
  }
  printf("\n");
  // test error
  cc = 15;
  fa1 = fopen("./log.txt", "w+");
  if (fa1 == NULL) printf("fa1 is null\n");
  error(3);
  // test getch
  fin = fopen("./infile.txt", "r");
  ll = 0;
  cc = 0;
  int ret1 = getsym();
  printf("input line: %s", line);
  printf("return code: %d\n", ret1);
  i = 0;
  while (line[i] != 0 && i < 81) {
    printf("%d ", line[i]);
    ++i;
  }
  printf("\n");
  fclose(fin);
  fclose(fa1);
}

int main(int argc, char** argv) {
  //trials(argc, argv);
  if (argc == 0) {
    printf("No input file specified\n");
    return 1;
  }
  compile(argv[0]);
  return 0;
}
