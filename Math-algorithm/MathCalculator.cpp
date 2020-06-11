#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <math.h>
using namespace std;

// 함수 헤더
void chTostr(wchar_t);
void classifySrc(string);
int priority(char);
int calGCD(int, int);
int calLCM(int, int);
void calStack();
void cal();
void permutation(int, int, int);
void combination(int, int, int, int);
int printPrimeNum(int,  int);
void clear_args();

// operation 구조체
typedef struct operation{
  int prior;
  string o;
}oper;

string baseStr; // 중위표현식
vector<string> baseSrc;
stack<int> number;
stack<oper> op;

vector<int> p_c; // 순열, 조합 출력을 위한 더미 벡터
vector<int> p_c_visit; // 순열, 조합 출력을 위한 더미 벡터
int p_c_res = 0;

// 입력받은 값 baseStr이라는 string값으로 변환
extern "C" __declspec(dllexport)
int calculator(wchar_t temp_wch[]){

  int ans;
  char temp_ch[wcslen(temp_wch)];

  for (int i = 0; i < wcslen(temp_wch); i++) {
    temp_ch[i] = temp_wch[i];
    baseStr+= temp_ch[i];
  }

  // 계산 실행
  classifySrc(baseStr);
  char chk = baseSrc[1][0]; // 어떤 연산을 할 지 정한다.

  // 사용 규격을 정해준다.
  if(chk == 'G' || chk == 'g'){
    ans = calGCD(stoi(baseSrc[0]),stoi(baseSrc[2]));
    clear_args();
    return ans;
  }
  else if(chk == 'L' || chk == 'l'){
    ans = calLCM(stoi(baseSrc[0]),stoi(baseSrc[2]));
    clear_args();
    return ans;
  }
  else if(chk == 'P' || chk == 'p'){
    p_c_visit = vector<int>(stoi(baseSrc[0])+1, 0);
    permutation(0,stoi(baseSrc[0]),stoi(baseSrc[2]));
    ans = p_c_res;
    clear_args();
    return ans;
  }
  else if(chk == 'C' || chk == 'c'){
    p_c_visit = vector<int>(stoi(baseSrc[0])+1, 0);
    combination(0,stoi(baseSrc[0]),stoi(baseSrc[2]),1);
    ans = p_c_res;
    clear_args();
    return ans;
  }
  else if(chk == 'M' || chk == 'm'){
    ans = stoi(baseSrc[0]);
    return ans;
  }
  else if(chk == 'R' || chk == 'r'){
    ans = printPrimeNum(stoi(baseSrc[0]),stoi(baseSrc[2]));
    clear_args();
    return ans;
  }
  else{
    calStack();
    ans = number.top();
    clear_args();
    return ans;
  }
  return 0;

}

void clear_args(){
  baseStr = "";
  baseSrc.clear();
  while(!number.empty()) number.pop();
  while(!op.empty()) op.pop();

  p_c.clear();
  p_c_visit.clear();
  p_c_res = 0;
}

//숫자, 연산자 분류 하기
void classifySrc(string src){

  string temp;
  for (int i = 0; i < src.size(); i++) {
    if(src[i] >= '0' && src[i] <= '9'){
      temp += src[i];
      if(i == src.size()-1) baseSrc.push_back(temp);
    }
    else{
      if(src[i-1] >= '0' && src[i-1] <= '9'){
        baseSrc.push_back(temp);
        temp = "";
      }
      string temp_op;
      temp_op += src[i];
      baseSrc.push_back(temp_op);
    }
  }
}

// 연산자의 우선순위 수치화
int priority(string op){
  if (op == "(") return 0;
  if (op == "+" || op == "-") return 1;
  if (op == "X" || op == "/") return 2;
  else return 3;
}

// 피연산자 두개와 연산자 하나 계산 하는 함수
void cal(){
  int res; // 이 함수를 통해 계산을 끝냈을 때의 결과
  int A = number.top();
  number.pop();
  int B = number.top();
  number.pop();
  string operand = op.top().o;
  op.pop();

  // 순서와 연산자에 따라 res에 계산해서 저장한다.
  if(operand == "+") res = B + A;
  else if(operand == "-") res = B - A;
  else if(operand == "X") res = B * A;
  else if(operand == "/") res = B / A;

  // 최종 계산된 res를 number 스택에 push한다.
  number.push(res);
}

// 스택에 연산자와 피연산자를 push하고 계산을 manage하는 함수
void calStack(){

  for (int i = 0; i < baseSrc.size(); i++) {
    string idx = baseSrc[i]; // baseSrc의 원소를 하나씩 가져온다.

    // 각 연산자 기호에 따라 기능을 달리해서 수행한다.
    if(idx == "("){
      op.push({0, idx});
    }
    else if(idx == ")"){
      // "(" 만날때까지 계산 진행
      while(op.top().o != "(")
        cal();
      op.pop();
    }
    else if(idx == "X" || idx == "/" || idx == "+" || idx == "-"){
      // 사칙연산 기호일 때 현재 연산자의 우선순위가 더 높아질때까지 계산한다.
      while(!op.empty() && op.top().prior >= priority(idx))
        cal();
      // 이제 연산자를 스택에 넣는다.
      op.push({priority(idx), idx});
    }
    else{
      // 숫자는 stoi로 integer로 바꾸어서 저장
      number.push(stoi(idx));
    }
  }

  // 최종적으로 남은 연산자를 계산해준다.
  while(!op.empty())
    cal();
}

// 최대공약수 구하기 - 유클리드 알고리즘
int calGCD(int a, int b){
  if(b == 0)  return a;
  else  return calGCD(b, a%b);
}

// 최소공배수
int calLCM (int a, int b){
  int val = calGCD(a, b);
  return val * (a/val) *(b/val);
}

//순열
void permutation (int level, int a, int b){

  if(level == b){
    for (int l = 0; l < b; l++) {
      cout << p_c[l] << " ";
    }
    cout << "\n";
    p_c_res += 1;
    return;
  }
  else{
    for (int i = 1; i <= a; i++) {
      if(p_c_visit[i] == 0){
        p_c_visit[i] = 1;
        p_c.push_back(i);
        permutation(level + 1, a, b);
        p_c_visit[i] = 0;
        p_c.pop_back();
      }
    }
  }
}

//  조합
void combination (int level, int a, int b, int min){

  if(level == b){
    p_c_res += 1;
    return;
  }
  else{
    for (int i = min; i <= a; i++) {
      if(p_c_visit[i] == 0){
        p_c_visit[i] = 1;
        p_c.push_back(i);
        combination(level + 1, a, b, i);
        p_c_visit[i] = 0;
        p_c.pop_back();
      }
    }
  }
}

int printPrimeNum(int a, int b){

  vector<int> res;
  bool res_trigger;

  for (size_t i = a; i <= b; i++) {
    res_trigger = true;
    for (int j = 2; j <= sqrt(i); j++) {
      if(i % j == 0){
        res_trigger = false;
        break;
      }
    }

    if((res_trigger || i == 2) && i != 1)
      res.push_back(i);
  }
  
  return res.size();
}
