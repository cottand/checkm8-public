#ifndef SYMBOL_H_
#define SYMBOL_H_

typedef enum e_symbol
{
  Undefined,
  Opcode,
  Immediate,
  Register,
  Constant,
  Address,
  LBracket,
  RBracket,
  Colon,
  Sign
} symbol;

#endif /* SYMBOL_H_ */
