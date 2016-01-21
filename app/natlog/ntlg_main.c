#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ntlg_hash.h"
#include "ntlg_list.h"


	static int 
write_file(struct ntlg_list_info *info, FILE *fd)
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


	int 
main(int  argc, char **argv)
{
	char *file1, *file2, *outpath;
	FILE *fd;
	int time;
	int err;
	char buf[128]={0};		
	ntlg_list_entry_t *tmp=NULL, *ntmp=NULL;

	if(argc != 4){
		printf("useage: natlog FILE1 FILE2 OUTPATH\n");
		return 0;
	}

	file1 = argv[1];
	file2 = argv[2];
	outpath = argv[3];

	if((err=ntlg_list_read_contrack(file2)) != 0)	
		return err;

	if((err=ntlg_hash_read_contrack(file1)) != 0)
		return err;
	strcpy(buf, outpath);
	strcat(buf, "/temp.txt");
	if (!(fd = fopen(buf, "w"))) {
		printf("Could not open configuration file %s exiting...", buf);
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
