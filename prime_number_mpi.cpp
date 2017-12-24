#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc,char* argv[]){
MPI_Status status;
const int range=100000;
int prim[range];
int core_id;
int num_cores;
int flag=0;
int num=0;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&core_id);
MPI_Comm_size(MPI_COMM_WORLD,&num_cores);

int NumPerCores = range / num_cores;
int s = core_id * NumPerCores;
int e = (core_id + 1) * NumPerCores;
if(core_id == num_cores - 1)
	e = range + 1;
if(core_id == 0)
	s=3;

//std::cout << core_id << "\t"<<s<<"\t"<<e<<endl;
for(int i=1;i<range;i++){prim[i]=0;}

for(int a=s; a<e ;a++){
	//prim[a]=0;
	flag=0;
	for(int i=2; i<(a/2+1);i++){
		if(a%i==0){flag+=1;}
		}
	if(flag==0){prim[a]=a;}
	}

if(core_id != 0){
	MPI_Send(&prim,range,MPI_INT,0,99,MPI_COMM_WORLD);
	}
if(core_id == 0){
	FILE *fp = NULL;
	fp = fopen("prim_number.txt","w");
	prim[2]=2;
	num=0;
	for(int i=0;i<range;i++){
		if(prim[i]!=0){
			num+=1;
			fprintf(fp,"%d\n",prim[i]);
			}
		}
	
	for(int id = 1; id < num_cores; id++){
		int prim2[range];
		MPI_Recv(&prim2,range,MPI_INT,id,99,MPI_COMM_WORLD,&status);
		for(int i=0;i<range;i++){
			if(prim2[i]!=0){
				num+=1;
				fprintf(fp,"%d\n",prim2[i]);
				}
			}
		}
	std::cout << "the number of prime number is " << num << endl;
	}
MPI_Finalize();
return 0;

}
