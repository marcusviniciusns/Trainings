

DROP TRIGGER SYSTEM.INC_MOVTIPO_TRIG
/
DROP TRIGGER SYSTEM.INC_CIDADES_TRIG
/
DROP TRIGGER SYSTEM.INC_CONTATOS_TRIG
/
DROP TRIGGER SYSTEM.INC_MOVIMENTO_TRIG
/
DROP TRIGGER SYSTEM.INC_CLIENTES_TRIG
/


ALTER TABLE SYSTEM.MOVIMENTO DROP CONSTRAINT RELATION_CLI_MOV
/
ALTER TABLE SYSTEM.CLIENTES DROP CONSTRAINT RELATION_CON_CLI
/
ALTER TABLE SYSTEM.CLIENTES DROP CONSTRAINT RELATION_CID_CLI
/
ALTER TABLE SYSTEM.MOVIMENTO DROP CONSTRAINT RELATION_MOVTIPO_MOV
/


DROP TABLE SYSTEM.MOV_TIPO CASCADE CONSTRAINTS
/
DROP TABLE SYSTEM.CIDADES CASCADE CONSTRAINTS
/
DROP TABLE SYSTEM.CONTATOS CASCADE CONSTRAINTS
/
DROP TABLE SYSTEM.MOVIMENTO CASCADE CONSTRAINTS
/
DROP TABLE SYSTEM.CLIENTES CASCADE CONSTRAINTS
/


DROP SEQUENCE SYSTEM.CIDADES_CID_ID_SEQ;
DROP SEQUENCE SYSTEM.CLIENTES_CLI_ID_SEQ;
DROP SEQUENCE SYSTEM.CONTATOS_CON_ID_SEQ;
DROP SEQUENCE SYSTEM.MOVIMENTO_MOV_ID_SEQ;
DROP SEQUENCE SYSTEM.MOV_TIPO_MT_ID_SEQ;


