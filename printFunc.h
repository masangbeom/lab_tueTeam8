void makeXY(int tempSize, char* XY_Value, int * valueXY);
void printOrderList(char * tb_st, int * commandUI, uint16_t *pos_x, uint16_t *pos_y, char * XY_Value, int * xySize, int * valueXY);
int printWaitUI(int * commandUI, uint16_t *pos_x, uint16_t *pos_y, int * new_flag);
void startDelivery(int * commandUI, uint16_t *pos_x, uint16_t *pos_y, int * valueXY, int * stopFlag);
void printConfirm(char * tb_st, int * tbSizeBLE, int *commandUI, int * countConfirm);

