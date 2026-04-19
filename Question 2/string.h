#ifndef STRING_HEADER
#define STRING_HEADER




inline int getLength(const char* str) {
    if(str == nullptr)  return 0;
    
    int length=0;
    while(*(str++)!='\0')
        length++;
    return length;
}

inline char* copyString(const char* str) {

    if(str == nullptr)
        return nullptr;
    int length = getLength(str);

    char* copy = new char[length+1];
    for(int i=0; i<length;i++){
        *(copy+i)=*(str+i);
    }
    *(copy+length)='\0';
    return copy;
} 

inline bool areEqual(const char* str1, const char* str2) {
    int length1=getLength(str1);
    int length2=getLength(str2);
    if(length1==0&&length2==0)    return 0;
    if(length1!=length2)    return 0;
    for(int i=0; i<length1;i++){

        if(*(str1+i)!=*(str2+i))
            return 0;
    }
    return 1;
    
}

inline bool isSmallerThan(const char* str1, const char* str2) {
    if(str1 == nullptr || str2 == nullptr) return 0;
    
    int i = 0;
    while(*(str1 + i) != '\0' && *(str2 + i) != '\0') {
        if(*(str1 + i) < *(str2 + i)) return 1;
        if(*(str1 + i) > *(str2 + i)) return 0;
        i++;
    }
    
    return (*(str1 + i) == '\0' && *(str2 + i) != '\0');
}

inline char* concatenate(const char* str1, const char* str2) {
    
    int length1=getLength(str1);
    int length2=getLength(str2);
    if(length1+length2==0) return nullptr;
    char* concatenated = new char[length1+length2+1];
    
    int index =0;
    for(int i=0; i<length1+length2;i++){
        if(i<length1&&str1!=nullptr){
            *(concatenated+index) = *(str1+i);
            index++;
        }
        else if(str2!=nullptr){
            *(concatenated+index) = *(str2+i-length1);
            index++;
        }   
    }
    *(concatenated+length1+length2) = '\0';


    return concatenated;
}

inline void toUpperCase(char* str) {
    if(str == nullptr)  return;

    int length = getLength(str);
    for(int i=0; i<length; i++){
        if(*(str+i)>='a'&&*(str+i)<='z'){
            *(str+i) -= 32;
            
        }
    }
}

inline void toLowerCase(char* str) {
    if(str == nullptr)  return;

    int length = getLength(str);
    for(int i=0; i<length; i++){
        if(*(str+i)>='A'&&*(str+i)<='Z'){
            *(str+i) += 32;
            
        }
    }    
}

inline int findFirstOccurrence(const char* str, char c) {
    if(str== nullptr)   return -1;
    int length = getLength(str);
    for(int i=0;i<length;i++){
        if(*(str+i)==c)
            return i;
    }
    return -1;
    
}

inline int findSubstring(const char* str, const char* sub) {
    if(str==nullptr||sub==nullptr) return -1;
    int lengthstr=getLength(str);
    int lengthsub=getLength(sub);
    if(lengthsub==0) return 0;
    if(lengthsub>lengthstr) return -1;

    for(int i=0;i<=lengthstr-lengthsub;i++){
        bool found=1;
        for(int j=0;j<lengthsub;j++){
            if(*(str+i+j)!=*(sub+j)){
                found=0;
                break;
            }
        }
        if(found) return i;
    }
    return -1;
    
}

inline char* reverseString(char* str){
    if(str==nullptr) return 0;
    int length=getLength(str);
    char* rev = new char[length+1];
    for(int i=0;i<length;i++){
        *(rev+i) = *(str+length -i-1);
    }
    *(rev + length) = '\0';
    return rev;
}

inline char* removeCharacter(char* str, char c) {
    if(str==nullptr) return 0;
    int length=getLength(str);
    int index=0;
    char* arr_del = new char[length+1];
    for(int i=0; i<length; i++){
        if(*(str+i)!=c){
        *(arr_del+index++) = *(str+i);
        }
    }
    *(arr_del+index) = '\0';
    return arr_del;
}

inline char* substring(const char* str, int start, int length) {
    int total_length = getLength(str);
    if(str==nullptr || start<0 || length<=0||start>=total_length||start+length>total_length) return nullptr;

    char* substr=new char[length+1];
    for(int i=0;i<length;i++){
        *(substr+i)=*(str+start+i);
    }
    *(substr+length)='\0';
    return substr;
}

inline char* replaceChar(char* str, char oldChar, char newChar){
    if(str==nullptr) return nullptr;
    int length=getLength(str);
    char* newstr = new char[length+1];
    for(int i=0;i<length;i++){
        if(*(str+i)==oldChar)
            *(newstr+i)=newChar;
        else 
            *(newstr+i) = *(str+i);
    }
    *(newstr + length ) = '\0';

    return newstr;
}


inline char* insertAt(const char* str, const char* sub, int pos) {
    int length=getLength(str);
    if(pos>length) return nullptr;
    int sublength = getLength(sub);
    char* newstr= new char[length+sublength+1];
    int index =0;
    for(int i=0; i<length; i++){
        if(i==pos){
            for(int j=0; j<sublength; j++){
                *(newstr + index++)=*(sub+j);
            }
        }
        *(newstr + index++)=*(str+i);
    }
    *(newstr+length+sublength)='\0';
    return newstr;
}

inline char* deleteRange(char* str, int start, int length) {
    int total_length=getLength(str);
    if(total_length<=length+start) length = total_length-start;
    if(start<0||length<=0)  return str;
    
    char* newstr= new char[total_length-length+1];
    int index =0;
    for(int i=0; i<total_length; i++){
        if(i==start)
        i+=length;
    *(newstr + index++)=*(str+i);
}
*(newstr+index)='\0';
return newstr;    
}


inline char* replaceSubstring(const char* str, const char* oldSub, const char* newSub) {
    if(str == nullptr || oldSub == nullptr || newSub == nullptr) return nullptr;
    
    int length = getLength(str);
    int oldSublength = getLength(oldSub);
    int newSublength = getLength(newSub);
    
    if(length == 0 || oldSublength == 0) return copyString(str);


    int count = 0;  
    for(int i = 0; i <= length-oldSublength; i++){
        if(findSubstring(str + i, oldSub) == 0){
            count++;
            i+=oldSublength-1;
        }
    }

    char* result = new char[length+count*(newSublength-oldSublength)+1];
    int j = 0, i = 0;
    
    while(i < length) {
        if(i <= length - oldSublength && findSubstring(str + i, oldSub) == 0) {
            for(int k = 0; k < newSublength; k++) *(result + j++) = *(newSub + k);
            i += oldSublength;
        } else {
            *(result + j++) = *(str + i++);
        }
    }
    
    *(result + j) = '\0';
    return result;
}

inline char* compress(const char* str) {
    if(str == nullptr) return nullptr;
    int len = getLength(str);
    if(len == 0) return nullptr;
    
    char* result = new char[len * 2 + 1];
    int j = 0;
    
    for(int i = 0; i < len; ) {
        char ch = *(str+i);
        int count = 1;
        while(i + count < len && *(str+i + count) == ch) count++;
        
        *(result+j++) = ch;
        *(result+j++)= '0' + count; 
        i += count;
    }
    
    *(result+j) = '\0';
    return result;
}

inline char* decompress(const char* str) {
    if(str == nullptr) return nullptr;
    int length = getLength(str);
    if(length == 0) return nullptr;
    
    char* newstr = new char[length * 10+1]; 
    int j = 0;
    
    for(int i = 0; i < length; i += 2) {
        char ch = *(str+i);          
        int count = *(str+i + 1) - '0'; 
        
        for(int k = 0; k < count; k++) {
            *(newstr+j++) = ch;         
        }
    }
    
    *(newstr+j) = '\0';
    return newstr;
}

inline char* rotateString(char* str, int k){
    if(str == nullptr) return nullptr;
    int length = getLength(str);
    if(length == 0) return nullptr;
    k=k%length;
    if(k<0) k+=length;

    char* newstr = new char[length+1];
    for(int i=0; i<length; i++){
        *(newstr + i) = *(str + i);
    }
    *(newstr+length) = '\0';
    for(int i=0; i<k; i++){
        char last = *(newstr);
        for(int j=0; j<length-1; j++){
            
            *(newstr+j)=*(newstr+j+1);
        }
        *(newstr+length-1) = last;
    }
    return newstr;
}

inline int  stoi(const char* str) {
    if (str == nullptr) return 0;
    
    int result = 0;
    int sign = 1;
    int i = 0;

    while (*(str + i) == ' ') i++;

    if (*(str + i) == '-') {
        sign = -1;
        i++;
    } else if (*(str + i) == '+') {
        i++;
    }

    while (*(str + i) >= '0' && *(str + i) <= '9') {
        result = result * 10 + (*(str + i) - '0');
        i++;
    }

    return result * sign;
}

inline float stof(const char* str) {
    if (str == nullptr) return 0.0f;

    float result = 0.0f;
    float sign = 1.0f;
    int i = 0;

    while (*(str + i) == ' ') i++;

    if (*(str + i) == '-') {
        sign = -1.0f;
        i++;
    } else if (*(str + i) == '+') {
        i++;
    }

    while (*(str + i) >= '0' && *(str + i) <= '9') {
        result = result * 10.0f + (*(str + i) - '0');
        i++;
    }

    if (*(str + i) == '.') {
        i++;
        float factor = 0.1f;
        while (*(str + i) >= '0' && *(str + i) <= '9') {
            result += (*(str + i) - '0') * factor;
            factor /= 10.0f;
            i++;
        }
    }

    return result * sign;
}

#endif