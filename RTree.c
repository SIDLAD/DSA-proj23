//this "https://www.cse.cuhk.edu.hk/~taoyf/course/infs4205/lec/rtree.pdf" is a better resource and deals with Rtrees with points.

#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
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


float calculateoverlap(float xl1, float yl1, float xh1, float yh1, float xl2, float yl2, float xh2, float yh2)
{
    // calculate the leftmost and rightmost x-coordinates of the overlap area
    float left = (xl1 > xl2) ? xl1 : xl2;
    float right = (xh1 < xh2) ? xh1 : xh2;

    // calculate the top and bottom y-coordinates of the overlap area
    float top = (yl1 > yl2) ? yl1 : yl2;
    float bottom = (yh1 < yh2) ? yh1 : yh2;

    // calculate the width and height of the overlap area
    float width = (right > left) ? (right - left) : 0.0;
    float height = (bottom > top) ? (bottom - top) : 0.0;

    // calculate and return the overlap area
    return width * height;
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
	else{
		float xl1=u[0]->I[0][0], yl1=u[0]->I[0][1], xh1=u[0]->I[1][0], yh1=u[0]->I[1][1];
		float xl2=u[1]->I[0][0], yl2=u[1]->I[0][1], xh2=u[1]->I[1][0], yh2=u[1]->I[1][1];
		for(int i=0;i<counter_c3;i++){
			if(C3[i]->I[0][0]<xl1)
				xl1=C3[i]->I[0][0];
			if(C3[i]->I[0][1]<yl1)
				yl1=C3[i]->I[0][1];
			if(C3[i]->I[1][0]>xh1)
				xh1=C3[i]->I[1][0];
			if(C3[i]->I[1][1]>yh1)
				yh1=C3[i]->I[1][1];
		}
		for(int i=0;i<counter_c1;i++){
			if(C1[i]->I[0][0]<xl2)
				xl2=C1[i]->I[0][0];
			if(C1[i]->I[0][1]<yl2)
				yl2=C1[i]->I[0][1];
			if(C1[i]->I[1][0]>xh2)
				xh2=C1[i]->I[1][0];
			if(C1[i]->I[1][1]>yh2)
				yh2=C1[i]->I[1][1];
		}
		float overlap_0=calculateoverlap(xl1,yl1,xh1,yh1,xl2,yl2,xh2,yh2);
		float area_0=(xh1-xl1)*(yh1-yl1)+(xh2-xl2)*(yl2-yl2)-overlap_0;
		
		xl1=u[0]->I[0][0]; yl1=u[0]->I[0][1]; xh1=u[0]->I[1][0]; yh1=u[0]->I[1][1];
		xl2=u[1]->I[0][0]; yl2=u[1]->I[0][1]; xh2=u[1]->I[1][0]; yh2=u[1]->I[1][1];
		for(int i=0;i<counter_c1;i++){
			if(C1[i]->I[0][0]<xl1)
				xl1=C1[i]->I[0][0];
			if(C1[i]->I[0][1]<yl1)
				yl1=C1[i]->I[0][1];
			if(C1[i]->I[1][0]>xh1)
				xh1=C1[i]->I[1][0];
			if(C1[i]->I[1][1]>yh1)
				yh1=C1[i]->I[1][1];
		}
		for(int i=0;i<counter_c3;i++){
			if(C3[i]->I[0][0]<xl2)
				xl2=C3[i]->I[0][0];
			if(C3[i]->I[0][1]<yl2)
				yl2=C3[i]->I[0][1];
			if(C3[i]->I[1][0]>xh2)
				xh2=C3[i]->I[1][0];
			if(C3[i]->I[1][1]>yh2)
				yh2=C3[i]->I[1][1];
		}
		float overlap_1=calculateoverlap(xl1,yl1,xh1,yh1,xl2,yl2,xh2,yh2);
		float area_1=(xh1-xl1)*(yh1-yl1)+(xh2-xl2)*(yl2-yl2)-overlap_1;
		
		if(overlap_0<overlap_1)
		{
		for(int i=0;i<=counter_c1-1;i++)
			fillcont(r,C1[i],u[1]);
		for(int i=0;i<=counter_c3-1;i++)
			fillcont(r,C3[i],u[0]);
		}
		else if(overlap_0>overlap_1){
		for(int i=0;i<=counter_c1-1;i++)
			fillcont(r,C1[i],u[0]);
		for(int i=0;i<=counter_c3-1;i++)
			fillcont(r,C3[i],u[1]);
		}
		else{
			if(area_0<area_1){
			for(int i=0;i<=counter_c1-1;i++)
				fillcont(r,C1[i],u[1]);
			for(int i=0;i<=counter_c3-1;i++)
				fillcont(r,C3[i],u[0]);
			}
			else{
			for(int i=0;i<=counter_c1-1;i++)
				fillcont(r,C1[i],u[0]);
			for(int i=0;i<=counter_c3-1;i++)
				fillcont(r,C3[i],u[1]);
			}
		}
	}
	//implemented the case when less than m number of nodes are present in node. see CBS paper.
	if(u[0]->size<m){
		int tmp_var=0;
		float x=(u[0]->I[0][0]+u[0]->I[1][0])/2, y=(u[0]->I[0][1]+u[0]->I[1][1])/2;
		float tmp_dist=powf(((u[1]->arr[0]->I[0][0]+u[1]->arr[0]->I[1][0])/2-x),2)+powf(((u[1]->arr[0]->I[0][1]+u[1]->arr[0]->I[1][1])/2-y),2);
		for(int i=1;i<u[1]->size;i++){
			float temp_area=(powf(((u[1]->arr[i]->I[0][0]+u[1]->arr[i]->I[1][0])/2-x),2)+powf(((u[1]->arr[i]->I[0][1]+u[1]->arr[i]->I[1][1])/2-y),2));
			if(tmp_dist>temp_area)
			{	tmp_dist=temp_area;
				tmp_var=i;
				
			}
		}
		u[0]->arr[u[0]->size]=u[1]->arr[tmp_var];
		for(int i=tmp_var+1;i<u[1]->size;i++)
			u[1]->arr[i-1]=u[1]->arr[i-1];
		u[1]->arr[u[1]->size-1]=NULL;
		u[1]->size--;
		
	}
	if(u[1]->size<m){
		int tmp_var=0;
		float x=(u[1]->I[0][0]+u[1]->I[1][0])/2, y=(u[1]->I[0][1]+u[1]->I[1][1])/2;
		float tmp_dist=powf(((u[0]->arr[0]->I[0][0]+u[0]->arr[0]->I[1][0])/2-x),2)+powf(((u[0]->arr[0]->I[0][1]+u[0]->arr[0]->I[1][1])/2-y),2);
		for(int i=1;i<u[0]->size;i++){
			float temp_area=(powf(((u[0]->arr[i]->I[0][0]+u[0]->arr[i]->I[1][0])/2-x),2)+powf(((u[0]->arr[i]->I[0][1]+u[0]->arr[i]->I[1][1])/2-y),2));
			if(tmp_dist>temp_area)
			{	tmp_dist=temp_area;
				tmp_var=i;
				
			}
		}
		u[1]->arr[u[1]->size]=u[0]->arr[tmp_var];
		for(int i=tmp_var+1;i<u[0]->size;i++)
			u[0]->arr[i-1]=u[0]->arr[i-1];
		u[0]->arr[u[0]->size-1]=NULL;
		u[0]->size--;
			
	}
	
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

int updateMBR(cont par_co, Node par, cont curr){
    int val=0;
    for(int i=0;i<2;i++)
    {
        if(par_co->I[0][i] > curr->I[0][i]){
            par->I[0][i] = curr->I[0][i];
            par_co->I[0][i] = curr->I[0][i];
            val=1;
            }
        if(par_co->I[1][i] < curr->I[1][i]){
            par->I[1][i] = curr->I[1][i];
            par_co->I[1][i] = curr->I[1][i];
            val=1;}
    }
    
    return val;
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
printf("hi\n");
	if(u0->arr[0]->parent==NULL)
		printf("adfef\n");
	(u0->arr[0]->parent)->I[0][0]=u0->I[0][0];
	(u0->arr[0]->parent)->I[0][1]=u0->I[0][1];
	(u0->arr[0]->parent)->I[1][0]=u0->I[1][0];
	(u0->arr[0]->parent)->I[1][1]=u0->I[1][1];
	assignchild(tmp->arr[0]->parent,tmp->arr[0]->par_cont,u0);
	Node new_tmp = createNewNode(u1->I[0][0],u1->I[0][1],u1->I[1][0],u1->I[1][1]);
	assignchild(new_tmp,tmp->arr[0]->par_cont,u1);
	bool tmp_leaf=u0->arr[0]->par_cont->isLeaf;
	u0->arr[0]->par_cont->isLeaf=1;
	fillcont(r, new_tmp, tmp->arr[0]->par_cont);
	u0->arr[0]->par_cont->isLeaf=tmp_leaf;
	
	printf("sizezzdwfwe: %d\n", tmp->arr[0]->par_cont->size);

	
		
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
	float area;
	float mbr;
	int tmp_i;
	float tmp_inc;
	Node t;
	float tmp_mbr;
	while(tmp->isLeaf==0){
	area=(tmp->arr[0]->I[1][0]-tmp->arr[0]->I[0][0])*(tmp->arr[0]->I[1][1]-tmp->arr[0]->I[0][1]);
	printf("perimeter: %f \n",area);
	if(tmp->arr[0]->I[0][0]<=point->I[0][0] && tmp->arr[0]->I[1][0]>=point->I[0][0]){
		if(tmp->arr[0]->I[0][1]<=point->I[0][1] && tmp->arr[0]->I[1][1]>=point->I[0][1])
			tmp_increase=0.0;
		else if(tmp->arr[0]->I[0][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-point->I[0][1])*(tmp->arr[0]->I[1][0]-tmp->arr[0]->I[0][0])-area;
		else
			tmp_increase=(point->I[0][1]-tmp->arr[0]->I[0][1])*(tmp->arr[0]->I[1][0]-tmp->arr[0]->I[0][0])-area;
	}
	else if(tmp->arr[0]->I[0][0]>=point->I[0][0]){
		if(tmp->arr[0]->I[0][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-point->I[0][1])*(tmp->arr[0]->I[1][0]-point->I[0][0])-area;
		else if(tmp->arr[0]->I[0][1]<=point->I[0][1] && tmp->arr[0]->I[1][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-tmp->arr[0]->I[0][1])*(tmp->arr[0]->I[1][0]-point->I[0][0])-area;
		else
			tmp_increase=(point->I[0][1]-tmp->arr[0]->I[0][1])*(tmp->arr[0]->I[1][0]-point->I[0][0])-area;
		}
	else if(tmp->arr[0]->I[1][0]<=point->I[0][0]){
				printf("zero\n");
		if(tmp->arr[0]->I[0][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-point->I[0][1])*(point->I[0][0]-tmp->arr[0]->I[0][0])-area;
		else if(tmp->arr[0]->I[0][1]<=point->I[0][1] && tmp->arr[0]->I[1][1]>=point->I[0][1])
			tmp_increase=(tmp->arr[0]->I[1][1]-tmp->arr[0]->I[0][1])*(point->I[0][0]-tmp->arr[0]->I[0][0])-area;
		else
			tmp_increase=(point->I[0][1]-tmp->arr[0]->I[0][1])*(point->I[0][0]-tmp->arr[0]->I[0][0])-area;
		}
	mbr=area;
	tmp_i=0;
	for(int i=1;i<tmp->size;i++){
		t=tmp->arr[i];


		area=(t->I[1][0]-t->I[0][0])*(t->I[1][1]-t->I[0][1]);
		if(t->I[0][0]<=point->I[0][0] && t->I[1][0]>=point->I[0][0]){
				if(t->I[0][1]<=point->I[0][1] && t->I[1][1]>=point->I[0][1])
					tmp_inc=0.0;
				else if(t->I[0][1]>=point->I[0][1])
					tmp_inc=((t->I[1][1]-point->I[0][1])*(t->I[1][0]-t->I[0][0]))-area;
				else
					tmp_inc=((point->I[0][1]-t->I[0][1])*(t->I[1][0]-t->I[0][0]))-area;
		}	
		else if(t->I[0][0]>=point->I[0][0]){
			if(t->I[0][1]>=point->I[0][1])
				tmp_inc=((t->I[1][1]-point->I[0][1])*(t->I[1][0]-point->I[0][0]))-area;
			else if(t->I[0][1]<=point->I[0][1] && t->I[1][1]>=point->I[0][1])
				tmp_inc=((t->I[1][1]-t->I[0][1])*(t->I[1][0]-point->I[0][0]))-area;
			else
				tmp_inc=((point->I[0][1]-t->I[0][1])*(t->I[1][0]-point->I[0][0]))-area;
			}
		else if(t->I[1][0]<=point->I[0][0]){
			if(t->I[0][1]>=point->I[0][1])
				tmp_inc=((t->I[1][1]-point->I[0][1])*(point->I[0][0]-t->I[0][0]))-area;
			else if(t->I[0][1]<=point->I[0][1] && t->I[1][1]>=point->I[0][1])
				tmp_inc=((t->I[1][1]-t->I[0][1])*(point->I[0][0]-t->I[0][0]))-area;
			else
				tmp_inc=((point->I[0][1]-t->I[0][1])*(point->I[0][0]-t->I[0][0]))-area;
			}
		tmp_mbr=area;
		if((tmp_inc)<(tmp_increase)){
			tmp_i=i;
			tmp_increase=(tmp_inc);
			mbr=tmp_mbr;
		}
		else if(abs(tmp_inc)==abs(tmp_increase)){
			if(tmp_mbr<=mbr){
				tmp_i=i;
				mbr=tmp_mbr;
			}
			
		}
		


	}


	tmp=tmp->arr[tmp_i]->child;
	printf("tmpinc: %f \n",tmp_increase);
	
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
tmp->arr[tmp->size-1]->parent=tmp->arr[0]->parent;
tmp->arr[tmp->size-1]->par_cont=tmp->arr[0]->par_cont;
}

if(r->Root==NULL)
r->Root=tmp;

cont x=tmp->arr[0]->par_cont;
Node y=tmp->arr[0]->parent;
cont curr=tmp;
int val=1;
while( x!=NULL && y!=NULL && val==1){
	val=updateMBR(x,y,curr);
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
    Node j= createNewNode(2,4,2,4);
    Node k= createNewNode(2,5,2,5);
    Node l= createNewNode(8,15,8,15);
    Node mo= createNewNode(8,14,8,14);
    Node n= createNewNode(7,15,7,15);
    Node o= createNewNode(9,14,9,14);
    Node p= createNewNode(9,15,9,15);
    Node q= createNewNode(9,16,9,16);
    Node r= createNewNode(9,17,9,17);
    Node s= createNewNode(12,17,12,17);
    Node t= createNewNode(11,18,11,18);
    Node u= createNewNode(1,20,1,20);
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
    printf("%f, %f, %f, %f \n", rt->Root->arr[0]->child->arr[0]->I[0][0],rt->Root->arr[0]->I[0][1],rt->Root->arr[0]->I[1][0],rt->Root->arr[0]->I[1][1]);
    fillcont(rt,h, rt->Root);
    fillcont(rt,i, rt->Root);
    fillcont(rt,j, rt->Root);
    fillcont(rt,k, rt->Root);
    fillcont(rt,l, rt->Root);
    fillcont(rt,mo, rt->Root);
    fillcont(rt,n, rt->Root);
    fillcont(rt,o, rt->Root);
    fillcont(rt,p, rt->Root);
    fillcont(rt,q, rt->Root);
    fillcont(rt,r, rt->Root);
    fillcont(rt,s, rt->Root);
    fillcont(rt,t, rt->Root);
    fillcont(rt,u, rt->Root);
    printf("%f, %f, %f, %f \n", rt->Root->I[0][0],rt->Root->I[0][1],rt->Root->I[1][0],rt->Root->I[1][1]);
    if(rt->Root->arr[1]->child->arr[0]==NULL)
    printf("affwwrgf\n");
    else
    printf("1111\n");
    printf("%d, %f, %f, %f \n", rt->Root->arr[2]->child->arr[0]->child->size,rt->Root->arr[0]->child->arr[0]->I[0][1],rt->Root->arr[0]->child->arr[0]->I[1][0],rt->Root->arr[0]->child->arr[0]->I[1][1]);
    return 0;
}
