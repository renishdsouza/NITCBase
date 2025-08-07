#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <iostream>
#include <cstring>
int main(int argc, char *argv[]) {
  /* Initialize the Run Copy of Disk */
  Disk disk_run;

  // create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // load the headers of both the blocks into relCatHeader and attrCatHeader.
  // (we will implement these functions later)
  
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);
  int attrCatSlotIndex=0;
  for (int i=0;i<relCatHeader.numEntries;i++/* i = 0 to total relation count */) {

    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

    for (int j=0;j<relCatRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal;j++,attrCatSlotIndex++/* j = 0 to number of entries in the attribute catalog */) {

      // declare attrCatRecord and load the attribute catalog entry into it
      Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

      attrCatBuffer.getRecord(attrCatRecord,attrCatSlotIndex);

      if (/* attribute catalog entry corresponds to the current relation */strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal)==0) {
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        if(strcmp(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,"Class")==0){
          // attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal="Batch";
          printf("  Batch: %s\n", /* get the attribute name */ attrType);
        }
        else
        printf("  %s: %s\n", /* get the attribute name */ attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,attrType);
      }

      if(attrCatSlotIndex == attrCatHeader.numSlots-1){
        attrCatSlotIndex= -1;
        attrCatBuffer= RecBuffer(attrCatHeader.rblock);
        attrCatBuffer.getHeader(&attrCatHeader);
      }
    }
    printf("\n");
  }

  return 0;
  // StaticBuffer buffer;
  // OpenRelTable cache;

  // return FrontendInterface::handleFrontend(argc, argv);
}