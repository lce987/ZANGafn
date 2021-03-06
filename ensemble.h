#define MAX 50

typedef struct{
  int ens[MAX];
}ensemble;

int est_ens(ensemble q[],ensemble e);
int est_etat(ensemble e,int n);
int eg_ens(ensemble ens1,ensemble ens2);
int aj_etat(ensemble *e,int n);
int etoi(ensemble e,int r[]);
void vider_ens(ensemble *e);
int aj_trans(ensemble t[][MAX],int etat_dep,char etiq,int etat_arr);
int trans(ensemble t[][MAX], int etat_dep, char etiq,ensemble *arr);
int ens_d(ensemble e,ensemble q_d[],int t_d[][MAX]);
