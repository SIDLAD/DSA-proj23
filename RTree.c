
//TODO: implement insert for non-leaf node which requires search-leaf function,
//TODO:  and implement overflow for non-root node. Then it's done max ~3 hrs work.

//This is clearly a better version than the one we theorized and with implementation of the above 2 TODOs, our assignment is over.

//Also CBS SPlit is completed, just have minor TODO in it too, with the tie break scenario.

//this "https://www.cse.cuhk.edu.hk/~taoyf/course/infs4205/lec/rtree.pdf" is a far better resource and deals with Rtrees with points. Following this helped me navigate the clutter created before.

#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdio.h>
/*Libraries to include above this*/

#define dim 2               //number of dimensions = 2
#define M 4
#define m 2
/*Macros defined above this*/


typedef struct rTree* RTree;
typedef struct node* Node;
typedef struct container* cont;

void fillcont(RTree r,Node node, cont tmp);
void assignchild(Node par, cont paren,cont chi);

typedef enum nodeType nodeType;
/*All typedefs above this*/

/*All enums defined above this*/
struct node
{
    Node parent;
    cont par_cont;
    float I[2][dim];          //Minimum Bounding region
    cont child;
    
};

struct cont_array
{
	cont arr[2];	
};


struct rTree
{
    cont Root;
};

struct container{
	Node arr[M+1];
	int size;
	bool isLeaf;
	float I[2][dim]; //Not a redundancy, it stores the MBR points, i.e(xh,xl,yh,yl)
};



RTree createNewRTree()
{
    RTree rtree = (RTree) malloc(sizeof(struct rTree));
    rtree->Root = NULL;
    return rtree;
}

Node createNewNode(float xl, float yl, float xh, float yh)
{
    Node node = (Node) malloc(sizeof(struct node));
    node->parent=NULL;
    node->child=NULL;
    node->par_cont=NULL;
    node->I[0][0]=xl;
    node->I[0][1]=yl;
    node->I[1][0]=xh;
    node->I[1][1]=yh;
    

    return node;
}


/*
int NodeLevel(Node node)
{
    //logic goes here
}

*/
cont createcont(int isitLeaf){
cont tmp=(cont)malloc(sizeof(struct container));
tmp->size=0;
tmp->isLeaf=isitLeaf;
return tmp;
}

struct cont_array cbs_split(RTree r,cont tmp){
Node C0[M];
int counter_c0=0;
Node C1[M];
int counter_c1=0;
Node C2[M];
int counter_c2=0;
Node C3[M];
int counter_c3=0;

float cov_x=(tmp->I[0][0]+tmp->I[1][0])/2;
float cov_y=(tmp->I[0][1]+tmp->I[1][1])/2;


for(int i=tmp->size-1;i>=0;i--){
float obj_x=(tmp->arr[i]->I[0][0]+tmp->arr[i]->I[1][0])/2;
float obj_y=(tmp->arr[i]->I[0][1]+tmp->arr[i]->I[1][1])/2;
if(obj_x>cov_x){
	if(obj_y>cov_y){
		C2[counter_c2]=tmp->arr[i];
		counter_c2++;
	}
	else{
		C3[counter_c3]=tmp->arr[i];
		counter_c3++;
	}		
}
else{
	if(obj_y>cov_y){
		C1[counter_c1]=tmp->arr[i];
		counter_c1++;
	}
	else{
		C0[counter_c0]=tmp->arr[i];
		counter_c0++;
	}
}
}

cont u[2];
u[0]=createcont(1);
u[1]=createcont(1);
u[0]->size=0;
u[1]->size=0;

if(counter_c0>counter_c2){
	for(int i=0;i<=counter_c0-1;i++)
		fillcont(r,C0[i],u[0]);
	for(int i=0;i<=counter_c2-1;i++)
		fillcont(r,C2[i],u[1]);
}
else{
	for(int i=0;i<=counter_c0-1;i++)
		fillcont(r,C0[i],u[1]);
	for(int i=0;i<=counter_c2-1;i++)
		fillcont(r,C2[i],u[0]);
}
if(counter_c1>counter_c3){
	for(int i=0;i<=counter_c1-1;i++)
		fillcont(r,C1[i],u[1]);
	for(int i=0;i<=counter_c3-1;i++)
		fillcont(r,C3[i],u[0]);
}
else{
	if(counter_c3>counter_c1){
		for(int i=0;i<=counter_c1-1;i++)
			fillcont(r,C1[i],u[0]);
		for(int i=0;i<=counter_c3-1;i++)
			fillcont(r,C3[i],u[1]);
	}
	//else should come here, see the paper //TODO: should implement tiebreak conditions of least overlap and least total coverage area.	
}
if(u[0]->arr[0]->child!=NULL){
	u[0]->isLeaf=0;
	u[1]->isLeaf=0;
	for(int i=0;i<u[0]->size;i++){
		assignchild(u[0]->arr[i],u[0],u[0]->arr[i]->child);
	}
	for(int i=0;i<u[1]->size;i++){
		assignchild(u[1]->arr[i],u[1],u[1]->arr[i]->child);
	}
}

struct cont_array new;
new.arr[0]=u[0];
new.arr[1]=u[1];

return new;
}

void updateMBR(cont par_co, Node par, cont curr){

    for(int i=0;i<curr->size;i++)
    {
        if(par->I[0][i] > curr->I[0][i]){
            par->I[0][i] = curr->I[0][i];
            par_co->I[0][i] = curr->I[0][i];
            }
        if(par->I[1][i] < curr->I[1][i]){
            par->I[1][i] = curr->I[1][i];
            par_co->I[1][i] = curr->I[1][i];}
    }
    
}


void overflow(RTree r,cont tmp){
struct cont_array u=cbs_split(r,tmp);
cont u0= u.arr[0];
cont u1= u.arr[1];
if(tmp->arr[0]->parent==NULL){
	Node new_root_1 = createNewNode(u0->I[0][0],u0->I[0][1],u0->I[1][0],u0->I[1][1]);
	Node new_root_2 = createNewNode(u1->I[0][0],u1->I[0][1],u1->I[1][0],u1->I[1][1]);
	cont new=createcont(1);
	fillcont(r,new_root_1,new);
	fillcont(r,new_root_2,new);
	new->isLeaf=0;
	r->Root=new;
	assignchild(new_root_1,new,u0);
	assignchild(new_root_2,new,u1);


}

else{
	(u0->arr[0]->parent)->I[0][0]=u0->I[0][0];
	(u0->arr[0]->parent)->I[0][1]=u0->I[0][1];
	(u0->arr[0]->parent)->I[1][0]=u0->I[1][0];
	(u0->arr[0]->parent)->I[1][1]=u0->I[1][1];
	
	for(int i=0;i<(tmp->size)-1;i++){
	Node node=tmp->arr[i];
	for(int j=0;j<2;j++){
	if(tmp->I[0][0]>node->I[j][0]){
		tmp->I[0][0]=node->I[j][0];
	}
	if(tmp->I[1][0]<node->I[j][0]){
		tmp->I[1][0]=node->I[j][0];
	}
	if(tmp->I[0][1]>node->I[j][1]){
		tmp->I[0][1]=node->I[j][1];
	}
	if(tmp->I[1][1]<node->I[j][1]){
		tmp->I[1][1]=node->I[j][1];
	}	
	}
	}

	Node new_tmp = createNewNode(u1->I[0][0],u1->I[0][1],u1->I[1][0],u1->I[1][1]);
	assignchild(new_tmp,tmp->arr[0]->par_cont,u1);
	tmp->arr[0]->par_cont->arr[tmp->arr[0]->par_cont->size]=new_tmp;
	tmp->arr[0]->par_cont->size++;
	cont x=tmp->arr[0]->par_cont;
	Node y=tmp->arr[0]->parent;
	cont curr=tmp;
	while( x!=NULL && y!=NULL){
		updateMBR(x,y,curr);
		x=x->arr[0]->par_cont;
		y=tmp->arr[0]->parent;
		curr=x;
		}
	if(tmp->arr[0]->par_cont->size==M+1)
	overflow(r, tmp->arr[0]->par_cont);	
	free(tmp);
	
		
}

}

void assignchild(Node par, cont paren,cont chi){
	for(int i=0;i<=chi->size-1;i++){
		(chi->arr[i])->parent=par;
		(chi->arr[i])->par_cont=paren;
		}
	par->child=chi;
}


cont chooseLeaf(cont temp,Node point){
//calculates the perimeter increase and choses the node with less perimeter increase. In case of tie, smallest MBR region is chosen
	
	cont tmp=temp;
	float tmp_increase;
	float perimeter;
	float mbr;
	int tmp_i;
	float tmp_inc;
	Node t;
	float tmp_mbr;
	while(tmp->isLeaf==0){
	perimeter=(tmp->arr[0]->I[1][0]-tmp->arr[0]->I[0][0])+(tmp->arr[0]->I[1][1]-tmp->arr[0]->I[0][1]);
	printf("perimeter: %f \n",perimeter);
	if(tmp->arr[0]->I[0][0]<=point->I[0][0] && tmp->arr[0]->I[1][0]>=point->I[0][0]){
		if(tmp->arr[0]->I[0][1]<=point->I[0][1] && tmp->arr[0]->I[1][1]>=point->I[0][1])
			tmp_increase=0.0;
		else if(tmp->arr[0]->I[0][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-point->I[0][1])+(tmp->arr[0]->I[1][0]-tmp->arr[0]->I[0][0])-perimeter;
		else
			tmp_increase=(point->I[0][1]-tmp->arr[0]->I[0][1])+(tmp->arr[0]->I[1][0]-tmp->arr[0]->I[0][0])-perimeter;
	}
	else if(tmp->arr[0]->I[0][0]>=point->I[0][0]){
		if(tmp->arr[0]->I[0][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-point->I[0][1])+(tmp->arr[0]->I[1][0]-point->I[0][0])-perimeter;
		else if(tmp->arr[0]->I[0][1]<=point->I[0][1] && tmp->arr[0]->I[1][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-tmp->arr[0]->I[0][1])+(tmp->arr[0]->I[1][0]-point->I[0][0])-perimeter;
		else
			tmp_increase=(point->I[0][1]-tmp->arr[0]->I[0][1])+(tmp->arr[0]->I[1][0]-point->I[0][0])-perimeter;
		}
	else if(tmp->arr[0]->I[1][0]<=point->I[0][0]){
				printf("zero\n");
		if(tmp->arr[0]->I[0][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-point->I[0][1])+(point->I[0][0]-tmp->arr[0]->I[0][0])-perimeter;
		else if(tmp->arr[0]->I[0][1]<=point->I[0][1] && tmp->arr[0]->I[1][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-tmp->arr[0]->I[0][1])+(point->I[0][0]-tmp->arr[0]->I[0][0])-perimeter;
		else
			tmp_increase=(point->I[0][1]-tmp->arr[0]->I[0][1])+(point->I[0][0]-tmp->arr[0]->I[0][0])-perimeter;
		}
	mbr=(tmp->arr[0]->I[1][0]-tmp->arr[0]->I[0][0])*(tmp->arr[0]->I[1][1]-tmp->arr[0]->I[0][1]);
	tmp_i=0;
	for(int i=1;i<tmp->size;i++){
		t=tmp->arr[i];


		perimeter=(t->I[1][0]-t->I[0][0])+(t->I[1][1]-t->I[0][1]);
		if(t->I[0][0]<=point->I[0][0] && t->I[1][0]>=point->I[0][0]){
				if(t->I[0][1]<=point->I[0][1] && t->I[1][1]>=point->I[0][1])
					tmp_inc=0.0;
				else if(tmp->arr[0]->I[0][1]>=point->I[0][1])
					tmp_inc=(t->I[1][1]-point->I[0][1])+(t->I[1][0]-t->I[0][0])-perimeter;
				else
					tmp_inc=(point->I[0][1]-t->I[0][1])+(t->I[1][0]-t->I[0][0])-perimeter;
		}	
		else if(t->I[0][0]>=point->I[0][0]){
			if(t->I[0][1]>=point->I[0][1])
				tmp_inc=(t->I[1][1]-point->I[0][1])+(t->I[1][0]-point->I[0][0])-perimeter;
			else if(t->I[0][1]<=point->I[0][1] && t->I[1][1]>=point->I[0][1])
				tmp_inc=(t->I[1][1]-t->I[0][1])+(t->I[1][0]-point->I[0][0])-perimeter;
			else
				tmp_inc=(point->I[0][1]-t->I[0][1])+(t->I[1][0]-point->I[0][0])-perimeter;
			}
		else if(t->I[1][0]<=point->I[0][0]){
			if(t->I[0][1]>=point->I[0][1])
				tmp_inc=(t->I[1][1]-point->I[0][1])+(point->I[0][0]-t->I[0][0])-perimeter;
			else if(t->I[0][1]<=point->I[0][1] && t->I[1][1]>=point->I[0][1])
				tmp_inc=(t->I[1][1]-t->I[0][1])+(point->I[0][0]-t->I[0][0])-perimeter;
			else
				tmp_inc=(point->I[0][1]-t->I[0][1])+(point->I[0][0]-t->I[0][0])-perimeter;
			}
		tmp_mbr=(t->I[1][0]-t->I[0][0])*(t->I[1][1]-t->I[0][1]);
		if(tmp_inc<tmp_increase){
			tmp_i=i;
			tmp_increase=tmp_inc;
			mbr=tmp_mbr;
		}
		else if(tmp_inc==tmp_increase){
			if(tmp_mbr<=mbr){
				tmp_i=i;
				mbr=tmp_mbr;
			}
			
		}
		


	}


	tmp=tmp->arr[tmp_i]->child;
	printf("tmpinc: %d \n",tmp_i);
	
	}

	
	return tmp;
}


void fillcont(RTree r,Node node, cont tmp){ //aka insertion
if(tmp->isLeaf==0){
	tmp=chooseLeaf(tmp,node);
}


if(tmp->size<=(M)){
tmp->arr[tmp->size]=node;
(tmp->size)++;
//this below calculation will avoid the need of traversing through all of its inner values to calculate MBR.


if(tmp->size==1){
tmp->I[0][0]=node->I[0][0];
tmp->I[0][1]=node->I[0][1];
tmp->I[1][0]=node->I[1][0];
tmp->I[1][1]=node->I[1][1];
}
else{

for(int j=0;j<2;j++){
	if(tmp->I[0][0]>node->I[j][0]){
		tmp->I[0][0]=node->I[j][0];
	}
	if(tmp->I[1][0]<node->I[j][0]){
		tmp->I[1][0]=node->I[j][0];
	}
	if(tmp->I[0][1]>node->I[j][1]){
		tmp->I[0][1]=node->I[j][1];
	}
	if(tmp->I[1][1]<node->I[j][1]){
		tmp->I[1][1]=node->I[j][1];
	}	
}
}

if(r->Root==NULL)
r->Root=tmp;

cont x=tmp->arr[0]->par_cont;
Node y=tmp->arr[0]->parent;
cont curr=tmp;
while( x!=NULL && y!=NULL){
	updateMBR(x,y,curr);
	x=x->arr[0]->par_cont;
	y=tmp->arr[0]->parent;
	curr=x;
	}


if(tmp->size==(M+1)){

overflow(r,tmp);

}
}



}





//For debugging purposes://
int main()
{
    Node a = createNewNode(1,9,1,9);
    Node b = createNewNode(2,20,2,20);
    Node c = createNewNode(2,19,2,19);
    Node d = createNewNode(3,20,3,20);
    Node e = createNewNode(2,10,2,10);
    Node f = createNewNode(8,5,8,5);
    Node g = createNewNode(4,5,4,5);
    Node h= createNewNode(3,4,3,4);
    Node i= createNewNode(3,5,3,5);
    cont first=createcont(1);
    RTree rt = createNewRTree();
    fillcont(rt,a, first);
    fillcont(rt,b, rt->Root);
    fillcont(rt,c, rt->Root);
    fillcont(rt,d, rt->Root);
    fillcont(rt,e, rt->Root);
    fillcont(rt,f, rt->Root);
        printf("%f, %f, %f, %f \n", rt->Root->arr[0]->I[0][0],rt->Root->arr[0]->I[0][1],rt->Root->arr[0]->I[1][0],rt->Root->arr[0]->I[1][1]);
    printf("g insert:%d\n",rt->Root->isLeaf);
    fillcont(rt,g, rt->Root);
        printf("h insert:%d\n",rt->Root->isLeaf);
        printf("%f, %f, %f, %f \n", rt->Root->arr[0]->I[0][0],rt->Root->arr[0]->I[0][1],rt->Root->arr[0]->I[1][0],rt->Root->arr[0]->I[1][1]);
    fillcont(rt,h, rt->Root);
    printf("%f, %f, %f, %f \n", rt->Root->arr[1]->I[0][0],rt->Root->arr[1]->I[0][1],rt->Root->arr[1]->I[1][0],rt->Root->arr[1]->I[1][1]);
    return 0;
}
