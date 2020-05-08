#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
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
void print_matrix(int, int);

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
    return calGCD(stoi(baseSrc[0]),stoi(baseSrc[2]));
  }
  else if(chk == 'L' || chk == 'l'){
    return calLCM(stoi(baseSrc[0]),stoi(baseSrc[2]));
  }
  else if(chk == 'P' || chk == 'p'){
    p_c_visit = vector<int>(stoi(baseSrc[0])+1, 0);
    permutation(0,stoi(baseSrc[0]),stoi(baseSrc[2]));
    return p_c_res;
  }
  else if(chk == 'C' || chk == 'c'){
    p_c_visit = vector<int>(stoi(baseSrc[0])+1, 0);
    combination(0,stoi(baseSrc[0]),stoi(baseSrc[2]),1);
    return p_c_res;
  }
  else if(chk == 'M' || chk == 'm'){
    print_matrix(stoi(baseSrc[0]),stoi(baseSrc[2]));
    return stoi(baseSrc[0]);
  }
  else{
    calStack();
    return number.top();
  }
  return 0;

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
  if (op == "*" || op == "/") return 2;
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
  else if(operand == "*") res = B * A;
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
    else if(idx == "*" || idx == "/" || idx == "+" || idx == "-"){
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
    for (int l = 0; l < b; l++) {
      printf("%d ", p_c[l]);
    }
    printf("\n");
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

void print_matrix(int a, int b){

  for (int i = 1; i <= b; i++) {
    printf("%4d x %4d = %4d\n", a, i, a*i);
  }

}
