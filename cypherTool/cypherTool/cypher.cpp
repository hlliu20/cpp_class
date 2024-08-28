
#include "cypher.h"

//char * caesarEncode(char* s, int n = -1) {
//	// 如果传入字符串指针为nullptr，返回空指针
//	if (s == nullptr) return nullptr;
//	// 计算传入字符串长度
//	int len = strlen(s);
//	// 传入字符串长度为0，直接返回
//	if (len == 0)return s;
//	// 如果第二个参数为-1，计算全部26种可能
//	if(n==-1){
//		int k = 0; // k 记录写入到新字符串的字符位数
//		char * ss = new char[(len + 4) * 26 + 1]; // 给输出结果分配空间
//		for (int i = 1; i <= 26; ++i) {
//			ss[k++] = (i/10 + '0'); ss[k++] = (i % 10 + '0'); ss[k++] = ':';
//			for (int j = 0; j < len; ++j) {
//				char c = s[j];
//				if (c >= 'A' && c <= 'Z') { ss[k++] = (c - 'A' + i) % 26 + 'A'; }
//				else if (c >= 'a' && c <= 'z') { ss[k++] = (c - 'a' + i) % 26 + 'a'; }
//				else { ss[k++] = c; }
//			}
//			ss[k++] = '\n';
//		}
//		ss[k] = '\0';
//		delete[] s;
//		return ss;
//	}
//	else {
//		for (int i = 0; i < len; ++i) {
//			char c = s[i];
//			if (c >= 'A' && c <= 'Z') { s[i] = (c - 'A' + n) % 26 + 'A'; }
//			else if (c >= 'a' && c <= 'z') { s[i] = (c - 'a' + n) % 26 + 'a'; }
//			else {}
//		}
//		return s;
//	}
//}

/// <summary>
/// 对字符串进行参数为1的凯撒加密
/// </summary>
/// <param name="s">密文 string的引用</param>
void strShiftOne(std::string & s) {
	for (int i = 0; i < s.length(); ++i) {
		char c = s.at(i);
		if ((c >= 'A') && (c <= 'Z')) { s[i] = (c - 'A' + 1) % 26 + 'A'; }
		else if ((c >= 'a') && (c <= 'z')) { s[i] = (c - 'a' + 1) % 26 + 'a'; }
		// else { s[i] = c; }
		
	}
}
/// <summary>
/// 凯撒密码-暴力求解
/// </summary>
/// <param name="strIn">密文 string</param>
/// <returns>“明文s” string</returns>
std::string caesar(std::string strIn) {
	if (strIn.size()==0) return "";
	int len = strIn.length();
	/*ostringstream ss;
	for (int i = 1; i < 26; ++i) {
		strShiftOne(strIn);
		ss << setw(2) << i << ":" << strIn << "\n";
	}
	return ss.str();*/
	std::string strOut;
	for (int i = 1; i < 26; ++i) {
		strShiftOne(strIn);
		if (i < 10) strOut.append(" ");
		strOut.append(to_string(i) + ":" + strIn + "\r\n");
	}
	return strOut;
}


char morse_dot = '.';
char morse_line = '-';
char morse_delimiter = ' ';
unordered_map<string, string>morse_en = {
	{"a",".-"},{"b","-..."},{"c","-.-."},{"d","-.."},{"e","."},
	{"f","..-."},{"g","--."},{"h","...."},{"i",".."},{"j",".---"},
	{"k","-.-"},{"l",".-.."},{"m","--"},{"n","-."},{"o","---"},
	{"p",".--."},{"q","--.-"},{"r",".-."},{"s","..."},{"t","-"},
	{"u","..-"},{"v","...-"},{"w",".--"},{"x","-..-"},{"y","-.--"},
	{"z","--.."},{"0","-----"},{"1",".----"},{"2","..---"},{"3","...--"},
	{"4","....-"},{"5","....."},{"6","-...."},{"7","--..."},{"8","---.."},
	{"9","----."},{"?","..--.."},{"/","-..-."},{".",".-.-.-"},{"@",".--.-."},
	{"-","-....-"},{"(","-.--."},{")","-.--.-"},{":","---..."},{",","--..--"},
	{";","-.-.-."},{"=","-...-"},{"'",".----."},{"\"",".-..-."},{"_","..--.-"},
	{"!","-.-.--"},{"$","...-..-"},
};
unordered_map<string, string>morse_de{
	{".-","a"},{"-...","b"},{"-.-.","c"},{"-..","d"},{".","e"},
	{"..-.","f"},{"--.","g"},{"....","h"},{"..","i"},{".---","j"},
	{"-.-","k"},{".-..","l"},{"--","m"},{"-.","n"},{"---","o"},
	{".--.","p"},{"--.-","q"},{".-.","r"},{"...","s"},{"-","t"},
	{"..-","u"},{"...-","v"},{".--","w"},{"-..-","x"},{"-.--","y"},
	{"--..","z"},{"-----","0"},{".----","1"},{"..---","2"},{"...--","3"},
	{"....-","4"},{".....","5"},{"-....","6"},{"--...","7"},{"---..","8"},
	{"----.","9"},{"..--..","?"},{"-..-.","/"},{".-.-.-","."},{".--.-.","@"},
	{"-....-","-"},{"-.--.","("},{"-.--.-",")"},{"---...",":"},{"--..--",","},
	{"-.-.-.",";"},{"-...-","="},{".----.","'"},{".-..-.","\""},{"..--.-","_"},
	{"-.-.--","!"},{"...-..-","$"},
};
string morseDecode(string strIn) {
	string re, tp;
	bool flag = false;
	for (int i = 0; i < strIn.size(); ++i) {
		if (strIn.at(i) != morse_delimiter) { tp += strIn.at(i); }
		else {
			if (morse_de.count(tp) != 0) {
				re += morse_de[tp];
			} else {
				re += tp;
				flag = true;
			}
			tp = "";
		}
	}
	if (tp != "") {
		if (morse_de.count(tp) != 0) {
			re += morse_de[tp];
		}
		else {
			re += tp;
			flag = true;
		}
	}
	if (flag) re += "\a\n部分无法解码，原样显示！";
	return re;
}
string morseEncode(string strIn) {
	string re,tp;
	bool flag = false;
	for (int i = 0; i < strIn.size(); ++i) {
		tp = strIn.at(i);
		if (morse_en.count(tp) != 0) {
			re += morse_en[tp];
		}
		else {
			re += strIn.at(i);
			flag = true;
		}
		re += morse_delimiter;
	}
	if (flag) re += "\a\n部分无法加密，原样显示！";
	return re;
}

string fence(string strIn) {
	string strOut, si, tp, tpp;
	for (int i = 0; i < strIn.size(); ++i) {
		if (strIn.at(i) != ' ') { si += strIn.at(i); }
	}
	unsigned int len = si.size(),i,j,k,r,l;
	for (i = 2; i < len; ++i) {
		if (i < 10) strOut += " ";
		strOut += to_string(i) + ":";
		tp = "";
		r = len % i;
		l = len / i;
		for (j = 0; j < l; ++j) {
			for (k = 0; k < i; ++k) {
				if (k < r) {
					tp += si.at(k*(l+1)+j);
				}
				else {
					tp += si.at(r*(l+1)+(k-r)*l+j);
				}
			}
		}
		for (j = 0; j < r; ++j) {
			tp += si.at(j*(l+1)+l);
		}
		strOut += tp + "\r\n";
	}
	return strOut;
}