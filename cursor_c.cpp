#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbHead.h"

int getNextPage(struct dbSysHead *head, long fid, long num) {

	int pageIdInDisk = mapPage(head, fid, num+1);

	return reqPage(head, pageIdInDisk);
}

void getNextRecord(struct dbSysHead *head, long mapNo, long offset, long recordLen, void *des) {

	memcpy(des, head->buff.data[mapNo]+offset, recordLen*sizeof(char));

}