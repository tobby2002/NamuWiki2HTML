#include <stdio.h>
#include "stdnamu.h"

#define CORE 3 //코어 수 - 1 
#define Csize  CORE*50*1000*1000

string ReadJSON(FILE *input){ //[1~] : 각 문서의 포인터 반환. [0] : 원래 문서의 포인터가 들어 있음.(2번째 로딩부터는 사용.)
	char *cache;
	 *document;
	if( (cache = (char *)malloc(Csize)) == NULL) return (string){0,0};
	if( (document = (int *)malloc(sizeof(int)*500000)) == NULL) return (string){0,0};
	int iv, a;
	for (a = iv = 0; (Csize - a) >10000; iv++) {
		document[iv] = &cache[a]; //{"n 에서 n이 반환됨. 
		while( (cache[a++] = getc(input)) != '\"' ) while( (cache[a++] = getc(input)) != '{' ); // EOF 도. feof();
	} // 마지막은 기록 안됨. 
	string doc = {document,(iv-1)};
	printf("%d,%d,%d\n",document,document[1],iv);
	return doc;
}

int *worker(string doc){
	printf("\n%d,%d\n",doc.p,doc.p[1]);
	int full, i;
	printf("%d",doc.len);
	int *Cdoc; //변환 데이터의 포인터 저장 
	if( (Cdoc = (int *)malloc(sizeof(int) * full)) == NULL) return NULL;
	full--;
	#pragma omp parallel for
	for (i=0; i < full; i++) {//namespace = (char)-48, 
			char *temp = doc.p[i];
			if(temp[0] != 'n')continue;
			int read = 24;
			while( temp[read++] != '\"' ) while( temp[read++] != ',' );
			string title = {&temp[24] , ((read-4)-24)};
			int read2 = doc.p[i+1] - doc.p[i];
			while( temp[read2--] != '[' )( temp[read2--] != '\"' );
			string text ={&temp[read+7] , ((read2-17) - (read+7))};
			
			int end = parse((int)(temp[12]-48),title,text,Cdoc[i]);
			//printf("(%c,%d,%c%c,%c)",temp[0],(temp[12]-48),temp[24],temp[25],temp[read+7]);
			//parse((int)(temp[12]-48),&temp[24],&temp[read+7],Cdoc[i]);//namespace,title,text,cdocpointer
			
			
			for (read = 0; read < title.len ;read++)printf("%c",temp[24+read]);
			printf("\n");
			char* ttl = title.p;
			for (read = 0; read < title.len ;read++)printf("%c",ttl[read]);
			if(&temp[24] != &title.p[0]) printf("aaa\n");
			printf("\n");
		}
	return Cdoc;
}


int JsonIO(){
	FILE *input;
	if((input = fopen("namu.json","r")) == NULL) return 1;
	string doc;
	int *cptr;
	doc = ReadJSON(input);
	if( (cptr = worker(doc)) == NULL) return 3;
	free (doc.p[0]);
	free (cptr);
	
	//send to pointer
	//while all the pointer is not null : 
	// until 40% of memory left
	//  read near 50MB
	//  find title, namespace, document
	//start new parse thread
	return 0;
}

int main(){
	//JsonIO
	int IOret = JsonIO();
	//start worker - (core - 1) threads
	
	if (IOret == 1) printf("덤프 파일이 없거나 손상되었습니다.");
	return IOret;
}
