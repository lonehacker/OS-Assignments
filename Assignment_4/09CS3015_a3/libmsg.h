struct str_message{
  char sender[10];
  char recipient[10];
  int time;
  char message[100];
  int valid;
};
struct reg{
  char p_name[10];
  int pid;
  int valid;
};

int registr(char *pname);

int sendmessage(char *recname, char *mesg);

int listmessages(void);

int readmessage(struct str_message *buffer );

int deregister();
