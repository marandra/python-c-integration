#define OPJ_PATH_LEN 4096

struct param {
  char filei[OPJ_PATH_LEN];
  char fileo[OPJ_PATH_LEN];
  char conti[OPJ_PATH_LEN];
  char conto[OPJ_PATH_LEN];
  char grupi[OPJ_PATH_LEN];
  char grupo[OPJ_PATH_LEN];
  char pathi[OPJ_PATH_LEN];
  char patho[OPJ_PATH_LEN];
  char formt[OPJ_PATH_LEN];
  int pagen;
};
/*typedef struct param param;*/

void printusage(char *);
int parseiarg(char *, struct param *);
int parse_cmdline(int *, int, char **, param *);
int convertimageh5(struct param);
int convertimage(struct param);
//herr_t op_func_dset (hid_t, const char*, const H5L_info_t*, void*);
//herr_t op_func_group (hid_t, const char*, const H5L_info_t*, void*);
int convertimagebatch(param);
