#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

#define WIDTH 74;//index를 기준으로 75byte이기때문에 74로설정.
#define LINE "-------------------------------------------------------------------------------------"
#define OPERATIONMENU "n: 다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료"

class TextEditor
{
private:
	vector<string> MainStorageOfAllWords;
	static TextEditor* s_instance;
	int CurrentPoint = 0;
	string consoleMessage = "(콘솔메시지)";
	stack<int> StorageOfPageStartingPoint;

	vector<int> StorageOfLineStartingPoint{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };//20줄이니까 20개의 0으로 초기화
	int isLastPage;
	int isFirstPage;
	int linecount;
	vector<int> StorageOfLineWordCount{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };//20줄이니까 20개의 0으로 초기화

	TextEditor(vector<string> MainStorageOfAllWords = {})
	{
		MainStorageOfAllWords = MainStorageOfAllWords;
	}
public:
	vector<string> get_StorageOfAllWords()
	{
		return MainStorageOfAllWords;
	}
	void set_StorageOfAllWords(vector<string>& v)
	{
		MainStorageOfAllWords = v;
	}
	static TextEditor* instance()
	{
		if (!s_instance)
			s_instance = new TextEditor;
		return s_instance;
	}
	void drawingViewPage();

	void drawingLine();

	void moveToNextPage();
	void moveToLastPage();
	void moveToPrePage();

	void resetPage();

	void inputManager();
	vector<string> split(const string& str, char delim);
	void insertWord(int LineNuber, int wordNumber, string world);
	void deleteWord(int LineNuber, int wordNumber);
	void searchWord(string targetWord);
	void changeWord(string targetWorld, string newWord);
	void storeAndExit();
};

TextEditor* TextEditor::s_instance = 0;


int main() {
	vector <string> TempStorageOfAllWords;
	ifstream in("test.txt");//file open with filename
	string s;

	if (in.is_open())
	{
		while (in >> s) {
			TempStorageOfAllWords.push_back(s);
		}
		in.close();
	}
	else cout << "Unable to open file";
	//test.txt파일을 vector에 저장

	TextEditor::instance()->set_StorageOfAllWords(TempStorageOfAllWords);
	TextEditor::instance()->moveToNextPage();

	while (1) {
		TextEditor::instance()->drawingViewPage();
	}
}

void TextEditor::drawingViewPage() {
	cout << LINE << endl;
	cout << OPERATIONMENU << endl;
	cout << LINE << endl;
	cout << consoleMessage << endl;
	cout << LINE << endl;
	inputManager();
};

void TextEditor::drawingLine() {
	string line;
	int countOfWord = 0;
	StorageOfLineStartingPoint[linecount] = CurrentPoint;//실제 줄번호로 인덱싱
	while (1) {
		if (CurrentPoint > MainStorageOfAllWords.size() - 1 && isLastPage == 0) {
			isLastPage = 1;
			CurrentPoint = StorageOfLineStartingPoint[linecount + 1];
			break;
		}
		if (CurrentPoint > MainStorageOfAllWords.size() - 1)
			break;
		else if (line.size() + MainStorageOfAllWords[CurrentPoint].size() > 74) {
			break;
		}
		line += MainStorageOfAllWords[CurrentPoint] + " ";
		countOfWord++;
		CurrentPoint++;
	}
	cout.width(3);
	cout << linecount << "| ";
	cout << line << endl;
	StorageOfLineWordCount[linecount++] = countOfWord;
};

void TextEditor::resetPage() {
	CurrentPoint = StorageOfPageStartingPoint.top();
	linecount = 1;
	for (int i = 0;i < 20;i++) {
		drawingLine();
	}
}

void TextEditor::moveToNextPage() {

	consoleMessage = "(콘솔메시지)";
	isFirstPage = 0;

	if (isLastPage == 1) {
		consoleMessage = "This is last Page!";
		CurrentPoint = StorageOfPageStartingPoint.top();
		moveToLastPage();
	}
	else {
		StorageOfPageStartingPoint.push(CurrentPoint);
		linecount = 1;
		for (int i = 0;i < 20;i++) {
			drawingLine();
			if (isLastPage == 1) {
				moveToLastPage();
				break;
			}
		}
	}
};

void TextEditor::moveToLastPage() {
	system("cls");
	StorageOfPageStartingPoint.pop();
	StorageOfPageStartingPoint.push(CurrentPoint);
	linecount = 1;
	for (int i = 0;i < 20;i++) {
		drawingLine();
	}
};

void TextEditor::moveToPrePage() {
	consoleMessage = "(콘솔메시지)";
	isLastPage = 0;

	if (StorageOfPageStartingPoint.size() == 1) {
		consoleMessage = "This is first Page";
		isFirstPage = 1;
		resetPage();
	}
	else {
		StorageOfPageStartingPoint.pop();
		resetPage();
	}
};

vector<string> TextEditor::split(const string& str, char delim) {
	vector<string> tokens;
	size_t start = 0, end = 0;
	while ((end = str.find(delim, start)) != string::npos) {
		tokens.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(str.substr(start));
	return tokens;
}

void TextEditor::inputManager() {
	vector<string> inputs;
	string input;
	cout << "입력: ";
	getline(cin, input);//공백까지 받기위함
	cout << LINE << endl;
	system("cls");
	if (input == "")//아무것도 입력되지 않는 경우
		input = "*";
	for (int i = 0;i < input.size() - 1;i++) {
		if (input[i] == ' ') {
			input[0] = '*';//입력오류처리
			break;
		}
	}
	if (input[0] == 'i' || input[0] == 'd' || input[0] == 'c' || input[0] == 's')
	{
		if (input[1] != '(' || input[input.size() - 1] != ')')
			input[0] = '*';//입력오류처리
	}
	if (input[0] == 'n' || input[0] == 'p' || input[0] == 't')
	{
		if (input.size()!=1)
			input[0] = '*';//입력오류처리
	}

	int countOfComma=0;
	int int_temp1;
	int int_temp2;
	string string_temp1;
	string string_temp2;
	switch (input[0]) {
	case 'i':
		for (int i = 1;i < input.size();i++) {
			if (input[i] == ',')
				countOfComma++;
		}
		if (countOfComma != 2) {
			consoleMessage = "잘못된 입력을 하셨습니다. 다시입력해주세요!";
			resetPage();
			break;
		}
		if (input[1] == '(' && input[input.length() - 1] == ')')
			inputs = split(input.substr(2, input.length() - 3), ',');

		if (atoi(inputs[0].c_str()) == 0 || atoi(inputs[1].c_str()) == 0 || inputs[2].length() >= 75) {//예외처리
			consoleMessage = "잘못된 입력을 하셨습니다. 다시입력해주세요!";
			resetPage();
			break;
		}

		int_temp1 = stoi(inputs[0]);
		int_temp2 = stoi(inputs[1]);
		string_temp1 = inputs[2];

		if (int_temp1 > 20 || int_temp2 > StorageOfLineWordCount[int_temp1]) {//예외처리
			consoleMessage = "잘못된 입력을 하셨습니다. 다시입력해주세요!";
			resetPage();
			break;
		}

		insertWord(int_temp1, int_temp2, string_temp1);
		break;
	case 'd':
		for (int i = 1;i < input.size();i++) {
			if (input[i] == ',')
				countOfComma++;
		}
		if (countOfComma != 1) {
			consoleMessage = "잘못된 입력을 하셨습니다. 다시입력해주세요!";
			resetPage();
			break;
		}
		if (input[1] == '(' && input[input.length() - 1] == ')')
			inputs = split(input.substr(2, input.length() - 3), ',');

		if (atoi(inputs[0].c_str()) == 0 || atoi(inputs[1].c_str()) == 0) {//예외처리
			consoleMessage = "잘못된 입력을 하셨습니다. 다시입력해주세요!";
			resetPage();
			break;
		}

		int_temp1 = stoi(inputs[0]);
		int_temp2 = stoi(inputs[1]);

		if (int_temp1 > 20 || int_temp2 > StorageOfLineWordCount[int_temp1]) {//예외처리
			consoleMessage = "잘못된 입력을 하셨습니다. 다시입력해주세요!";
			resetPage();
			break;
		}

		deleteWord(int_temp1, int_temp2);
		break;
	case 'c':
		for (int i = 1;i < input.size();i++) {
			if (input[i] == ',')
				countOfComma++;
		}
		if (countOfComma != 1) {
			consoleMessage = "잘못된 입력을 하셨습니다. 다시입력해주세요!";
			resetPage();
			break;
		}
		if (input[1] == '(' && input[input.length() - 1] == ')')
			inputs = split(input.substr(2, input.length() - 3), ',');

		if (inputs[1].length() >= 75) {//예외처리
			consoleMessage = "잘못된 입력을 하셨습니다. 다시입력해주세요!";
			resetPage();
			break;
		}

		string_temp1 = inputs[0];
		string_temp2 = inputs[1];
		changeWord(string_temp1, string_temp2);
		break;
	case 's':
		if (input[1] == '(' && input[input.length() - 1] == ')')
			string_temp1 = input.substr(2, input.length() - 3);
		searchWord(string_temp1);
		break;
	case 'n':
		moveToNextPage();
		break;
	case 'p':
		moveToPrePage();
		break;
	case 't':
		storeAndExit();
		break;
	default:
		consoleMessage = "잘못된 입력을 하셨습니다. 다시입력해주세요!";
		resetPage();
		break;
	}

};
void TextEditor::insertWord(int LineNuber, int wordNumber, string world) {
	consoleMessage = "(콘솔메시지)";
	int count = 0;
	int targetPoint = StorageOfLineStartingPoint[LineNuber] + wordNumber;

	for (vector<string>::iterator it = MainStorageOfAllWords.begin();it != MainStorageOfAllWords.end();it++) {
		if (count == targetPoint) {
			MainStorageOfAllWords.insert(it, world);
			break;
		}
		else {
			count++;
		}
	}
	resetPage();
};

void TextEditor::deleteWord(int LineNuber, int wordNumber) {
	consoleMessage = "(콘솔메시지)";
	int count = 0;
	int targetPoint = StorageOfLineStartingPoint[LineNuber] + wordNumber - 1;

	for (vector<string>::iterator it = MainStorageOfAllWords.begin();it != MainStorageOfAllWords.end();it++) {
		if (count == targetPoint) {
			MainStorageOfAllWords.erase(it);
			break;
		}
		else {
			count++;
		}
	}
	resetPage();
};
void TextEditor::searchWord(string targetWord) {
	consoleMessage = "(콘솔메시지)";
	int count;
	for (count = 0; count < MainStorageOfAllWords.size();count++) {
		if (MainStorageOfAllWords[count] == targetWord) {
			break;
		}
		else {
			count++;
		}
	}
	if (count >= MainStorageOfAllWords.size()) {
		consoleMessage = "There is no matching word";
		resetPage();
	}
	else {
		StorageOfPageStartingPoint.push(count);
		resetPage();
	}
};
void TextEditor::changeWord(string targetWorld, string newWord) {
	consoleMessage = "(콘솔메시지)";
	int count;
	vector<int> storageOfTargetWord;
	for (count = 0; count < MainStorageOfAllWords.size();count++) {
		if (MainStorageOfAllWords[count] == targetWorld)
			storageOfTargetWord.push_back(count);
		count++;
	}
	if (storageOfTargetWord.size() == 0) {
		consoleMessage = "There is no matching word";
		resetPage();
	}
	else {

		for (auto i : storageOfTargetWord) {

			MainStorageOfAllWords[i] = newWord;

		}
		resetPage();
	}
};
void TextEditor::storeAndExit() {
	ofstream out("test.txt");

	
	if (out.is_open()) {
		for (auto i : MainStorageOfAllWords) {
			out << i;
			out << " ";
		}
	}

	exit(0);
};