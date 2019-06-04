#ifndef ENCODE_DATA_TRANS_H_
#define ENCODE_DATA_TRANS_H_

#include "instruction.h"
#include "token_stream.h"

Data_Trans_Instr *encode_data_trans_instr(Token_Stream *instr);
void encode_data_trans_instr_pre_indexing(Data_Trans_Instr *encoded, Token_Stream *instr);
//void encode_data_trans_instr_shifted_register(Data_Trans_Instr *encoded, Token_Stream *instr);

void encode_data_trans_instr_shifted_register(Data_Trans_Instr *encoded, Token_Stream *instr);

#endif /* ENCODE_DATA_TRANS_H_ */
