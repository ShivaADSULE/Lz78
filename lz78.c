#include<stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#define MAXCHAR 100
char msg[1000];
char hex_msg[3000];





// INPUT FORMATING
void Read_File(){
    FILE *fp;
    char str[MAXCHAR];
    char* filename = "encode.txt";
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return;
    }
    while (fgets(str, MAXCHAR, fp) != NULL){
        strcat(msg, str);
    }
    fclose(fp);
}
void int_to_hex(int n,int j){
    char res[2]="00";
    int tmp =0,i=0;
    while(n!=0){
        tmp = n%16;
        if(tmp<10){
            res[i]='0'+tmp;
        }else{
            res[i]='a'+tmp-10;
        }
        i++;
        n/=16;
    }
    hex_msg[j]=res[1];
    hex_msg[j+1]=res[0];
}
void msg_to_hex(){
    int i = 0,tmp,j=0,c;
    for(i=0;i<strlen(msg);i++){
        tmp = msg[i];
        int_to_hex(tmp,j);
        j+=2;
    }
}

// Dictionary
typedef struct {
    char *key;
    int value;
} KVPair;
typedef struct Dictionary_t {
    KVPair *head;
    struct Dictionary_t *tail;
} Dictionary;
Dictionary* dict_new() {
    Dictionary *dictionary = (Dictionary*)malloc(sizeof(Dictionary));
    assert(dictionary != NULL);
    dictionary->head = NULL;
    dictionary->tail = NULL;
    return dictionary;
}
void dict_add(Dictionary *dictionary, const char *key, int value) {
    if(dict_has(dictionary, key))
            dict_remove(dictionary, key);
    if (dictionary->head != NULL) {
        while(dictionary->tail != NULL) {
            dictionary = dictionary->tail;
        }
        Dictionary *next = dict_new();
        dictionary->tail = next;
        dictionary = dictionary->tail;
    }
    int key_length = strlen(key) + 1;
    dictionary->head = (KVPair*)malloc(sizeof(KVPair));
    assert(dictionary->head != NULL);
    dictionary->head->key = (char *)malloc(key_length * sizeof(char));
    assert(dictionary->head->key != NULL);
    strcpy(dictionary->head->key, key);
    dictionary->head->value = value;
}
int dict_has(Dictionary *dictionary, const char *key) {
    if (dictionary->head == NULL)
        return 0;
    while(dictionary != NULL) {
        if(strcmp(dictionary->head->key, key) == 0)
            return 1;
        dictionary = dictionary->tail;
    }
    return 0;
}
int dict_get(Dictionary *dictionary, const char *key) {
    if (dictionary->head == NULL)
        return 0;
    while(dictionary != NULL) {
        if(strcmp(dictionary->head->key, key) == 0)
            return dictionary->head->value;
        dictionary = dictionary->tail;
    }
    return 0;
}
void dict_remove(Dictionary *dictionary, const char *key) {
    if (dictionary->head == NULL)
        return;
    Dictionary *previous = NULL;
    while(dictionary != NULL) {
        if(strcmp(dictionary->head->key, key) == 0) {
            if(previous == NULL) {
                free(dictionary->head->key);
                dictionary->head->key = NULL;
                if (dictionary->tail != NULL) {
                    Dictionary *toremove = dictionary->tail;
                    dictionary->head->key = toremove->head->key;
                    dictionary->tail = toremove->tail;
                    free(toremove->head);
                    free(toremove);
                    return;
                }
            }
            else {
                previous->tail = dictionary->tail;
            }
            free(dictionary->head->key);
            free(dictionary->head);
            free(dictionary);
            return;
        }
        previous = dictionary;
        dictionary = dictionary->tail;
    }
}
void dict_free(Dictionary *dictionary) {
    if(dictionary == NULL)
        return;
    free(dictionary->head->key);
    free(dictionary->head);
    Dictionary *tail = dictionary->tail;
    free(dictionary);
    dict_free(tail);
}








void downSplit(char str[],char tmp[],int s){
    for(int i=0;i<s;i++){
        tmp[i] = str[i];
    }
}

void upSplit(char str[],char tmp[],int s){
    for(int i=s;i<strlen(str);i++){
        tmp[i] = str[i];
    }
}


/*
dict_add(dictionary, "key", value);
dict_get(dictionary, "key");
*/

void lz78_encode(char msg[],int curr,int idx,Dictionary* dict){
    if(dict_get(dict,msg)){
        printf(" %d , %c \n",dict_get(dict,msg),msg[strlen(msg)-1]);
        return;
    }
    char tmp[500];
    downSplit(msg,tmp,curr);
    if(dict_get(tmp)==0){
        printf(" 0 , %c \n",tmp[strlen(tmp)-1]);
        dict_add(dict,tmp,idx);
    }
    curr+=1;
    lz78_encode(tmp,curr,idx+1,dict);
}



int main() {
    int i;
    Dictionary *dict= dict_new();
    Read_File();
    msg_to_hex();
    for(i=0;i<strlen(hex_msg);i++)
        printf("%c",hex_msg[i]);
    lz78_encode(hex_msg,1,dict);
    return 0;
}
