
extern "C" {
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbHead.h"
};
extern "C" {
extern int reqPage(struct  dbSysHead *head, long query);
extern long mapPage(struct dbSysHead *head, long fid, long num);
}

#include <iostream>

using namespace std;

class RecordCursor
{
public:
	// construct function
	RecordCursor(struct dbSysHead *head, long fid, long recordLength) {
		int idx = queryFileID(head, fid);
		if(idx < 0) {
			cout << fid << "file doesn't exist!" << endl;
			exit(0);
		}
		this->head = head;
		this->fid = fid;
		this->recordLength = recordLength;
		this->cLogicPage = -1;
		this->cBufferPage = 0;
		this->bufferOffset = 0;
		this->start = true;
		this->filePageNum = head->desc.fileDesc[fid].filePageNum;
	}

	bool getNextRecord(void *des);

private:
	struct dbSysHead *head;
	long fid;
	long cLogicPage;
	long filePageNum;
	long cBufferPage;
	long bufferOffset;
	long recordLength;
	bool start;
};

bool RecordCursor::getNextRecord(void *des) {
	// all the data is read
	if(this->cLogicPage >= this->filePageNum) {
		return false;
	}
	// if the next record is out of the buffer page
	// then read the next page into buffer
	if(this->start==true || (this->bufferOffset+recordLength)>SIZE_PER_PAGE) {
		this->cLogicPage += 1;
		long pageID = mapPage(this->head, this->fid, this->cLogicPage);
		this->cBufferPage = reqPage(this->head, pageID);
		this->bufferOffset = 0;
		this->start = false;
	}
	// read the record into des
	memcpy(des, this->head->buff.data[this->cBufferPage]+bufferOffset, sizeof(char)*recordLength);
	this->bufferOffset += recordLength;

	return true;
}



// int getNextPage(struct dbSysHead *head, long fid, long num) {

// 	int pageIdInDisk = mapPage(head, fid, num+1);

// 	return reqPage(head, pageIdInDisk);
// }

// int getNextRecord(struct dbSysHead *head, long mapNo, long offset, long recordLen) {

// 	return head->buff.data[mapNo][offset:offset+recordLen]
// }