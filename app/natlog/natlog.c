#define CAPP 1

#if !CAPP
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "hash_list.h"
#else
#ifndef __THIS_APP
#define __THIS_APP     natlog 
#endif

#include "utils.h"

OS_INITER;
#endif

struct ntlg_info{
	char procotol[8];
	char privip[20];
	char privport[8];
	char pubip[20];
	char pubport[8];
	char dstip[20];
	char dstport[8];
};

typedef struct ntlg_list_entry{
	struct list_head list;
	struct ntlg_info info;
}ntlg_list_entry_t;

typedef struct ntlg_hash_entry{
	struct ntlg_info info;
	struct hlist_node node;
}ntlg_hash_entry_t;


#define NTLG_HASH_SIZE 100000
#define ntlg_list_each_entry_safe(tmp, ntmp)  \
	list_for_each_entry_safe(tmp,ntmp, &ntlg_list,list)  


struct hlist_head ntlg_hash_head[NTLG_HASH_SIZE];

#if !CAPP
#ifndef OS_STRING_BKDR_NUMBER
#define OS_STRING_BKDR_NUMBER   31
#endif
#endif

LIST_HEAD(ntlg_list);

#if !CAPP
	static inline uint32_t
__bkdr_push(uint32_t a, uint32_t b)
{
	return a * OS_STRING_BKDR_NUMBER + b;
}

	static inline uint32_t
__binary_bkdr(const unsigned char *binary, uint32_t len)
{
	uint32_t hash = 0;

	if (binary) {
		int i;

		for (i=0; i<len; i++) {
			hash = __bkdr_push(hash, *(binary + i));
		}
	}

	return hash;
} 
#endif

	static int 
ntlg_hash_head_init(void)
{
	int i;
	for(i=0; i<=NTLG_HASH_SIZE; i++)
		INIT_HLIST_HEAD(&ntlg_hash_head[i]);	
	return 0;
}

	static int
get_key_value(char *line, char *key, int klen, char *value, int vlen)
{
	char *p;
	if((p=strstr(line, "=")) == NULL)
		return 1;
	else{
		*p='\0';
		p++;
		memset(key, 0, klen);
		memset(value, 0, vlen);
		strncpy(key, line, klen-1);
		strncpy(value, p, vlen-1);
	}
	return 0;
}

	static int 
parse_conntrack_line(char *line, struct ntlg_info *info)
{
	char buf[20][128]={0};
	char key[20]={0};
	char value[20]={0};
	int i=0;

	char *p=strtok(line, " ");
	while(p != NULL){
		if(i >= 20)
			return 1;
		if((i==2) && strcmp(p, "tcp") && strcmp(p, "udp"))
			return -1;
		strcpy(buf[i++], p);
		p = strtok(NULL, " ");
	}

	if(!strcmp(buf[2], "tcp")){

		if(!strcmp(buf[10], "[UNREPLIED]")) 
			return -1;

		strcpy(info->procotol, buf[3]);		


		if(get_key_value(buf[6],key, sizeof(key), value, sizeof(value)) || strcmp(key, "src"))
			return 1;
		strcpy(info->privip, value);

		if(get_key_value(buf[8],key, sizeof(key), value, sizeof(value)) || strcmp(key, "sport"))
			return 1;
		strcpy(info->privport, value);
		if(get_key_value(buf[11],key, sizeof(key), value, sizeof(value)) || strcmp(key, "dst"))
			return -1; 
		strcpy(info->pubip, value);

		if(get_key_value(buf[13],key, sizeof(key), value, sizeof(value)) || strcmp(key, "dport"))
			return -1; 
		strcpy(info->pubport, value);

		if(get_key_value(buf[7],key, sizeof(key), value, sizeof(value)) || strcmp(key, "dst"))
			return 1;
		strcpy(info->dstip, value);

		if(get_key_value(buf[9],key, sizeof(key), value, sizeof(value)) || strcmp(key, "dport"))
			return 1;	
		strcpy(info->dstport, value);
	}
	else if(!strcmp(buf[2], "udp")){

		if(!strcmp(buf[9], "[UNREPLIED]"))	
			return -1;

		strcpy(info->procotol, buf[3]);		


		if(get_key_value(buf[5],key, sizeof(key), value, sizeof(value)) || strcmp(key, "src"))
			return 1;
		strcpy(info->privip, value);

		if(get_key_value(buf[7],key, sizeof(key), value, sizeof(value)) || strcmp(key, "sport"))
			return 1;
		strcpy(info->privport, value);

		if(get_key_value(buf[10],key, sizeof(key), value, sizeof(value)) || strcmp(key, "dst"))
			return 1; 
		strcpy(info->pubip, value);

		if(get_key_value(buf[12],key, sizeof(key), value, sizeof(value)) || strcmp(key, "dport"))
			return 1; 
		strcpy(info->pubport, value);

		if(get_key_value(buf[6],key, sizeof(key), value, sizeof(value)) || strcmp(key, "dst"))
			return 1;
		strcpy(info->dstip, value);

		if(get_key_value(buf[8],key, sizeof(key), value, sizeof(value)) || strcmp(key, "dport"))
			return 1;
		strcpy(info->dstport, value);		

	}else{
		return -1;
	}
	return 0;
}

	static unsigned int  
get_head(struct ntlg_info *info)
{
	unsigned int hash=0;
	hash = __binary_bkdr((char *)info, sizeof(struct ntlg_info));
	return (hash%NTLG_HASH_SIZE);
}


	static int 
is_info_equal(struct ntlg_info *a, struct ntlg_info *b)
{
	if(strcmp(a->procotol, b->procotol))	
		return 0;
	if(strcmp(a->privip, b->privip))
		return 0;
	if(strcmp(a->privport, b->privport))
		return 0;
	if(strcmp(a->pubip, b->pubip))
		return 0;
	if(strcmp(a->pubport, b->pubport))
		return 0;
	if(strcmp(a->dstip, b->dstip))
		return 0;
	if(strcmp(a->dstport, b->dstport))
		return 0;
	return 1;
}

	static struct ntlg_hash_entry * 
get_item(struct ntlg_info *info)
{
	struct ntlg_hash_entry *item;
#if 1 
	int n = get_head(info);
	hlist_for_each_entry(item, &ntlg_hash_head[n], node)
	{
		if(is_info_equal(&(item->info), info)){
			return item;
		}
	}
#endif
	return NULL;
}


	int 
ntlg_hash_read_contrack(char *pathfile)
{
	int err;
	FILE *fd;
	char line[1024]={0};

	if (!(fd = fopen(pathfile, "r"))) {
		printf("Could not open configuration file %s exiting...", pathfile);
		return(1);
	}		

	while (!feof(fd) && fgets(line, sizeof(line), fd)) {

		ntlg_hash_entry_t *hash_node = malloc(sizeof(ntlg_hash_entry_t));		
		memset(hash_node, sizeof(ntlg_hash_entry_t), 0);

		err = parse_conntrack_line(line, &hash_node->info);
		if(err == 1){
			free(hash_node);
			fclose(fd);
			return 1;
		}
		else if(err == 0)
			hlist_add_head(&hash_node->node, &ntlg_hash_head[get_head(&hash_node->info)]);		
		else
			free(hash_node);
	}
	fclose(fd);
	return 0;
}
	int  
ntlg_hash_can_get_item(struct ntlg_info *info)
{

	struct ntlg_hash_entry *p;
	p = get_item(info);
	if(p == NULL)	
		return 0;	
	return 1;
}
int hlist_free_all(void)
{
	int i;
	struct ntlg_hash_entry *item;
	struct hlist_node *ntmp;
	for(i=0; i<NTLG_HASH_SIZE; i++){
		if(ntlg_hash_head[i].first != NULL){
			item = NULL;
			hlist_for_each_entry_safe(item, ntmp,&ntlg_hash_head[i], node){
				hlist_del(&item->node);
				free(item);
			}
		}
	}

	return 0;
}


	static int
ntlg_list_read_contrack(char *pathfile)
{
	int err;
	FILE *fd;
	char line[1024]={0};

	if (!(fd = fopen(pathfile, "r"))) {
		printf("Could not open configuration file %s exiting...", pathfile);
		return(1);
	}

	while (!feof(fd) && fgets(line, sizeof(line), fd)) {
		ntlg_list_entry_t *entry = (ntlg_list_entry_t *)malloc(sizeof(ntlg_list_entry_t));
		memset(entry, sizeof(ntlg_list_entry_t), 0);

		err = parse_conntrack_line(line, &entry->info);
		if(err == 1){
			printf("ntlg failed with line: %s\n", line);
			free(entry);
			fclose(fd);
			return 1;
		}
		else if(err == 0){
			list_add_tail(&entry->list,&ntlg_list);
		}
		else
			free(entry);
	}

	fclose(fd);
	return 0;
}
	static int 
list_free_all(struct list_head *list_head)
{

	struct ntlg_list_entry *tmp, *ntmp;

	list_for_each_entry_safe(tmp, ntmp, list_head,list){
		list_del(&tmp->list);
		free(tmp);
	}
	return 0;
}

	static int 
write_file(struct ntlg_info *info, FILE *fd)
{
	char buf[1024]={0};
	int err;  
	snprintf(buf, sizeof(buf), "%s %s %s %s %s %s %s\n", \
			info->procotol, info->privip, info->privport, info->pubip, info->pubport, info->dstip, info->dstport);
	printf("buf=%s\n", buf);
	err = fwrite(buf, strlen(buf)+1, 1, fd);	
	if(err != 1){
		return err;
	}

	return 0;
}

#ifndef __BUSYBOX__
#define natlog_main  main
#endif

int natlog_main(int  argc, char **argv)
{
	char *file1, *file2, *outpath;
	FILE *fd;
	int time;
	int err;	
	ntlg_list_entry_t *tmp=NULL, *ntmp=NULL;

	if(argc != 4){
		printf("useage: natlog FILE1 FILE2 OUTPATH\n");
		return 0;
	}

	file1 = argv[1];
	file2 = argv[2];
	outpath = argv[3];

	if((err=ntlg_list_read_contrack(file2)) != 0){	
		list_free_all(&ntlg_list);
		return err;
	}
	if((err=ntlg_hash_read_contrack(file1)) != 0){
		list_free_all(&ntlg_list);
		hlist_free_all();
		return err;
	}

	if (!(fd = fopen(outpath, "w"))) {
		list_free_all(&ntlg_list);
		hlist_free_all();
		printf("Could not open configuration file %s exiting...", outpath);
		return(1);
	}	

	ntlg_list_each_entry_safe(tmp, ntmp){
		if(!ntlg_hash_can_get_item((struct ntlg_info *)&tmp->info)){
			write_file(&tmp->info, fd);
		}
		list_del(&tmp->list);
		free(tmp);
	}

	fclose(fd);
	hlist_free_all();
	return 0;

}
