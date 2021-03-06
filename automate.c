#include <stdio.h>
#include <stdlib.h>
#include "automate.h"
#include "afficheur.h"
#include <string.h>

void A_defaut(Automate *A){
  A->q.ens[0]=3;
  A->q.ens[1]=A->q.ens[2]=A->q.ens[3]=1;

  A->a[0]=2;
  A->a[1]='a';
  A->a[2]='b';
  A->t[0][1].ens[0]='a';
  A->t[0][2].ens[0]='b';

  A->t[0][0].ens[0]=A->q.ens[0];
  A->t[0][0].ens[1]=A->a[0];
  aj_trans(A->t,1,'a',1);
  aj_trans(A->t,1,'b',1);
  aj_trans(A->t,1,'b',2);
  aj_trans(A->t,2,'b',3);
  aj_trans(A->t,3,'a',3);
  aj_trans(A->t,3,'b',3);


  A->i.ens[0]=1;
  A->i.ens[1]=1;

  A->f.ens[0]=1;
  A->f.ens[3]=1;

}

int A_fichier(Automate *A){
  int i=0,j,k,n,etat1,etat2;
  char ligne[50][20],donnee[30];
  FILE *f;
  char c,etiq;
  fprintf(stdout,"Modifiez le fichier \"loader\" pour insérez votre automate\n");

  f = fopen("loader","r");
  if(f==NULL){
    fprintf(stderr,"Le fichier ne peut pas etre ouvert\n");
    return 0;
  }
  A->q.ens[0]=A->a[0]=A->i.ens[0]=A->f.ens[0]=0;

  A->t[0][0].ens[1]=0;
  do{
    fgets(ligne[i],50,f);
    if(ligne[i][0]=='E' && ligne[i][1]=='O' && ligne[i][2]=='F'){
      break;
    }
    fgets(donnee,50,f);
    sscanf(ligne[i],"%c%d:",&c,&n);

    for(j=0;j<n;j++){
      if(c=='Q'){
        A->t[0][0].ens[0]=n;
        sscanf(donnee,"%d ",&etat1);
        aj_etat(&A->q,etat1);
        donnee[j*2]=' ';
      }
      if(c=='a'){
        sscanf(donnee,"%c",&etiq);
        A->a[0]++;
        A->a[A->a[0]-0]=etiq;   // -0 pour convertir code ascii en int
        A->t[0][A->a[0]-0].ens[0]=etiq;
        for(k=0;k<n-j;k++){
          donnee[k*2]=donnee[(k+1)*2];
        }
      }
      if(c=='t'){
        A->t[0][0].ens[1]=A->a[0];
        sscanf(donnee,"%d%c%d ",&etat1,&etiq,&etat2);
        aj_trans(A->t,etat1,etiq,etat2);
        donnee[j*4]=donnee[j*4+1]=donnee[j*4+2]=' ';
      }

      if(c=='i'){

        sscanf(donnee,"%d",&etat1);
        aj_etat(&A->i,etat1);
        donnee[j*2]=' ';
      }
      if(c=='f'){

        sscanf(donnee,"%d",&etat1);
        aj_etat(&A->f,etat1);
        donnee[j*2]=' ';
      }
    }

    i++;
  }
  while(1);
  i=1;
  while(A->a[i]!='\0'){
    A->t[0][i].ens[0]=A->a[i];
    i++;
  }
  A->t[0][0].ens[1]=A->a[0];
  fclose(f);
  return 1;
}

int init_aut(Automate *A){
  /*int x;
  fprintf(stdout,"Quel type de saisie voulez vous?\n1- Automate de base (par defaut)\n2- Lecture par fichier\n");
  fscanf(stdin,"%d",&x);
  if(x!=1 && x!=2){
    fprintf(stdout, "Choix indisponible automate par défaut choisi");
  }
  if(x==2){
    while(A_fichier(A)==0);
    return 2;
  }*/
  A_fichier(A);
  return 1;
}

int tr_finaux(ensemble f,ensemble q_d[],ensemble *f_d){
  int fin[MAX],fin_d[MAX],i,j,k;
  etoi(f, fin);
  for(i=1; i<=fin[0];i++ ){
    for(j=1; j<=q_d[0].ens[0]; j++){
      etoi(q_d[j], fin_d);
      for(k=1; k<=fin_d[0];k++){
        if(fin_d[k]==fin[i]){
          aj_etat(f_d, j);
        }
      }
    }
  }
  return 1;
}

int res_trans_d(Automate A,int t_d[][MAX], ensemble q_d[]){

  int i,j,k,dep[10];
  ensemble e;

  for(i=1;i<=q_d[0].ens[0];i++){
    for(j=1;j<=A.a[0];j++){
      etoi(q_d[t_d[i][0]],dep);
      vider_ens(&e);
      for(k=1;k<=dep[0];k++){
        trans(A.t,dep[k],A.a[j],&e);
      }
      t_d[i][j]=ens_d(e,q_d,t_d);
    }
  }
  return 1;
}

// utilisation de l'automate
int det_aut(Automate A,Automate_d *Ad){

  int i;
  ens_d(A.i,Ad->q_d,Ad->t_d);
  Ad->i_d=ens_d(A.i,Ad->q_d,Ad->t_d);   // état initial dans afn l'est aussi dans afd
  for(i=0;i<=Ad->t_d[0][0];i++){
    Ad->t_d[0][i]=A.a[i];
  };
  res_trans_d(A,Ad->t_d,Ad->q_d);
  strcpy(Ad->a,A.a);
  tr_finaux(A.f,Ad->q_d,&Ad->f_d);   //trouver finaux
  return 1;
}

int rec_mot(Automate A,char mot[]){
  int i,j;
  int etats[50][50];
  ensemble e;
  etoi(A.i,etats[0]);
  for(i=0;i<strlen(mot);i++){
    etats[i+1][0]=0;
    for(j=1;j<=etats[i][0];j++){
      trans(A.t,etats[i][j],mot[i],&e);
      etoi(e,etats[i+1]);
    }
  }
  for(j=1;j<=etats[i][0];j++){
    if(est_etat(A.f,etats[i][j])){
      fprintf(stdout,"Reconnu %s\n",mot);
      return 1;
    }
  }
  fprintf(stdout,"Non Reconnu %s\n",mot);
  return 0;
}
