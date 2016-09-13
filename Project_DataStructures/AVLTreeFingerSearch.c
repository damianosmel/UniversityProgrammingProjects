#include <stdio.h>
#include <stdlib.h>
#include<time.h>

//create a struct that represents a tree's node
struct nod {
	int level[2];
    int children;
    int key[4];
    int paidia[5];
    int injured;
    int parent;
    int riza;
    int is_deleted;
}*nodes;
   
int m;
/*create a global variable i to hold the position of x the return of access(x)
and the count_nod showing the candidate node for deletion*/
unsigned int i,count_nod=0; 

int access(int x,int tr);
void insert(int x);
void delete_nod(int x);
void split(int v);
void share(int x,int v,int u);
void fuse(int x,int v,int u);
int f_search(int x,int f);

int main(void){
	long int j,g,komb_f;
    int k,c,d,apant,f,found_a,found_f;
    double xr1,xr2,xr3,xr4,time_access,time_finger;
       
    printf("\n Please insert number of leaves to the initial tree \n");
    scanf("%ld",&m);
    
    if( (nodes = (struct nod*)malloc(100000 * sizeof(struct nod))) == NULL ) //allocate the memory for the biggest tree that will be used
    {
		printf("\n\nMemory couldn't be allocated ");
    }//having 100000 nodes the maximum number of leaves is 50000
    
    int a[m];
    for (j=0;j<=99999;j++){ //initialize tree nodes
		nodes[j].children=0;
        nodes[j].injured=0;
        nodes[j].is_deleted=1;
        
        for(k=0;k<=3;k++){
			nodes[j].key[k]=-1;
            nodes[j].paidia[k]=-1;
        }
        nodes[j].paidia[4]=-1;
        nodes[j].parent=-1;
        nodes[j].riza=0;
    }
    system("pause");
    
    //set up the values of each node
    for(c=0;c<=m-1;c++){
		a[c]=c+1; 
        printf("\n a[%d] holds the value %d",c,a[c]);             
    }
    
    //create the starting tree with the root and two leaves !!
    nodes[0].children=2;
    nodes[0].key[0]=a[0];
    nodes[0].paidia[0]=1;
    nodes[0].paidia[1]=2;
    nodes[0].riza=1;
    nodes[0].is_deleted=0;
    nodes[0].level[0]=-1;
    nodes[0].level[1]=-1;
    
    //set up the left leaf
    nodes[1].children=0;
    nodes[1].paidia[0]=a[0];
    nodes[1].parent=0;
    nodes[1].is_deleted=0;
    nodes[1].level[0]=-1;
    nodes[1].level[1]=2;
    
    //set up the right leaf
    nodes[2].children=0;
    nodes[2].parent=0;
    nodes[2].paidia[0]=a[1];
    nodes[2].is_deleted=0;
    nodes[2].level[0]=1;
    nodes[2].level[1]=-1;
    
    for(k=3;k<=m-1;k++){ //insert the values of the tree nodes to the tree
       insert(a[k]);              
    }     
        
    print_nod();//print tree nodes information
    system("PAUSE");
    do{//now, start the do while loop for tree function selected by user
		printf("\n Please select tree function: 1 insert(x), 2 delete(x), 3 search (with two algorithms), 0 to finish \n");             
        scanf("%d",&apant);
        switch(apant)
        {
            case 1: //insertion case
				printf(" Please give element for insertion in the tree ");             
                scanf("%d",&d);
                insert(d); 
                m++;     
                break;     
            case 2: //deletion case
                printf(" Please give element for deletion from the tree ");
                scanf("%d",&d);
                delete_nod(d);  
                m--;         
                break;
            case 3: //search case
                 printf(" Please give element to search for in the tree ");
                 scanf("%d",&d);
                 printf("\n Please give finger value for finger search in the tree ");
                 scanf("%d",&f);
                 
                 //start timer and run benchmark the access time for value to be searched for
                 xr1=(double)clock();
                 found_a=access(d,-2); 
                 for(g=1;g<=1000000;g++)
                 {
					access(d,-2);                      
                 }
                 xr2=(double)clock();
                 time_access=(double)(((xr2-xr1)*1000)/CLOCKS_PER_SEC);
                 
                 //start timer and run benchmark for finger searching the value
                 komb_f=access(f,-2);
                 xr3=(double)clock();
                 found_f=f_search(d,komb_f);
                 
                 for(g=1;g<=1000000;g++)
                 {
					f_search(d,komb_f);                      
                 } 
                 xr4=(double)clock();
                 time_finger=(double)( ((xr4-xr3)*1000)/CLOCKS_PER_SEC); 
                 
                 if(found_a!=-1){// as finger search calls access I do not need to check if both function have found the value
					printf("\n The value %d was found by the access function after %f nsec",d,time_access);
                    printf("\n The value %d was found by the finger search after %f nsec \n",d,time_finger);                                
                 }
                 else{
                     printf("\n The value %d is not found at the tree, the search time of access function was %f nsec",d,time_access);
                     printf("\n The value %d is not found at the tree, the search time of finger search function was %f nsec\n",d,time_finger);
                 }
            system("pause");
            break;           
                                 
           default :
                 break; 
          } //end of switch cases  
          print_nod(); 
         system("PAUSE");           
      }while(apant);
     
      system("PAUSE");
      return 0;
  }//end of main
   
//function to print all tree nodes information
print_nod(){
	int k,b,rootFound=0;
    long int c=0;
    int count_nod,count_leaves,telos,count_del;
    
    count_nod=count_leaves=0;
    count_del=0;
    maximumNumberOfLeaves=(m/10)*5;
    
    do{//for each node print the respective information
		printf("\n <--------------------------> ");     
        if(nodes[c].is_deleted==0 ){ //if node is not deleted      
			if(nodes[c].children!=0){ //and has children (=>internal node) 
				if(nodes[c].riza==1){ //and it has the flag being root!
					printf("\n The root of the tree!!!");       
                    rootFound=1;         
				}
				//if the node is not root then print its keys and children          
                count_nod++;                    
                printf("\n We are at position %d, where the %d node is located \n",c,count_nod);
                printf("It has the keys:\n");
                for(k=0;k<=2;k++){
                    if(nodes[c].key[k]!=-1){              
						printf(" Its' %d key has value %d ,",k+1,nodes[c].key[k]);              
                    }
                 }
                 printf("\n This node has the following children:\n");  
                 for(k=0;k<=3;k++){  
					if(nodes[c].paidia[k]!=-1){               
						printf(" Its' %d child is located at position %d,",k+1,nodes[c].paidia[k]);               
                    }
                 } 
                 if(rootFound){
					system("pause");   
                    rootFound=0;
                 }
            }
            else{//if the node does not children (=>leaf node)
				count_leaves++;
                printf("\n We are position %d, where the %d leaf is located \n",c,count_leaves);
                printf("(Does not have keys)\n");
                printf("It has value %d",nodes[c].paidia[0]);  
            }
            printf("\n It points to the left to %d and to the right to %d",nodes[c].level[0],nodes[c].level[1]);
            printf("\n It has father the node %d ",nodes[c].parent); 
	    } 
        else{//if node is deleted
            printf("\n The position %d is deleted",c);  
            count_del++;
        }    
        printf("\n <--------------------------> \n");
        c++;
        if(count_del>maximumNumberOfLeaves){ //if deleted nodes are all the leaves then finish printing information
			break;}
       }while(c<=99999);               
}//end of print node information

//implement the finger search algorithm
int f_search(int x,int fing){
    int kateu8,komb_anaz=fing,mpampa;
    int searchMove; 
    int  a,ch;
    
    //find search direction depending on finger value
	if(nodes[fing].paidia[0]>x){
		kateu8=0;
    }
    else if(nodes[fing].paidia[0]<x){
		kateu8=1;
    }
    else{
		return fing;  
    }
       
	for(searchMove=0;;searchMove++){
		if(!searchMove){//if we are not at the first search move
			mpampa=nodes[komb_anaz].parent; 
			ch=nodes[mpampa].children-1;
			if(x>=nodes[nodes[mpampa].paidia[0]].paidia[0]&&x<=nodes[nodes[mpampa].paidia[ch]].paidia[0]){//return the father of finger as result of search
				return access(x,mpampa);
			}
        }
        else if(searchMove==1){
			komb_anaz=nodes[mpampa].level[kateu8];    
            ch=nodes[komb_anaz].children-1;
            if(x>=nodes[nodes[komb_anaz].paidia[0]].paidia[0]&&x<=nodes[nodes[komb_anaz].paidia[ch]].paidia[0]){//move the search direction and return the father of current node 
				return access(x,komb_anaz);                                                                                        
            }
            mpampa=nodes[komb_anaz].parent;  
            komb_anaz=mpampa;
        }
        else{//if we are at least at second search move
			ch=nodes[komb_anaz].children-2;
            if(nodes[komb_anaz].riza==1){//if we reached the root of tree, return it
				return access(x,komb_anaz);
            }
            else{//if not at root try to access the respective node using the current node's keys
				if(x>=nodes[komb_anaz].key[0]&&x<nodes[nodes[komb_anaz].level[kateu8]].key[0]){
					a=access(x,komb_anaz);
					if(a==-1){
						return access(x,nodes[komb_anaz].level[kateu8]);       
					}
					else{
						return a;  
					}
				}  
                komb_anaz=nodes[komb_anaz].level[kateu8];
            }
         }   
    }//close for     
 }//end of finger search
 
//implement access function
int access(int x,int tr){
	//do to a up to bottom search, firstly find the root 
	for(i=0;;i++){
		if(nodes[i].riza==1) break;      
    }
    //if finger search uses the access() start the search from the level of the finger value
    if(tr!=-2){
		i=tr;
    }
    
    int r;
    do{ //start do while loop to search tree for the value
		int done=0;
		//try to find the value from the keys of current node 
        for(r=0;(done==0&&r<=2);r++){
			if(nodes[i].key[r]!=-1){
				if (x<=nodes[i].key[r]){
					i=nodes[i].paidia[r]; 
                    done=1;          
                }
            }
        }
        //an einai megalutero apo ola t kleidia
        if(!done){//if value bigger than all keys, go the right most children of current node
			i=nodes[i].paidia[nodes[i].children-1];     
        }
    }while(i!=-1&&nodes[i].children!=0);
        
    if(x==nodes[i].paidia[0]){//if you found value, return node index
		return i;
    }
    else{//otherwise return failure value
		return -1;
    }
}//end of access

//implement insert action
void insert(int x){
	int found=access(x,-2); //before inserting value search it on tree
    int temp,value;
    int nod_ins,j;
    
    if(found<0){//if value is not found, then insert it
		nod_ins=nodes[i].parent;
        printf("\n Father of node %d will get the new insertion", nod_ins);
        //change current node left and right pointer for finger search
        int left = nodes[nodes[nod_ins].paidia[0]].level[0];
        int right = nodes[nodes[nod_ins].paidia[nodes[nod_ins].children-1]].level[1];
        
        //find the node where the value will be inserted as a leaf
        value=x-1;    
        for(j=0;j<=3;j++){   
			if(nodes[nod_ins].key[j]==-1){
				nodes[nod_ins].key[j]=value;
                break;
            } 
            else if(x<=nodes[nod_ins].key[j]){
				temp=nodes[nod_ins].key[j];
                nodes[nod_ins].key[j]=value;   
                value=temp; 
            }    
        } 
        //change nodes properties due to insertion
        nodes[nod_ins].children++;
        nodes[nod_ins].injured=(nodes[nod_ins].children>4)?1:0;

        for(;(nodes[count_nod].is_deleted==0);count_nod++){//searching for the index of the new leaf
        }
        
        value=count_nod;
        for(j=0;j<=4;j++){   
			if(nodes[nod_ins].paidia[j]==-1){
				nodes[nod_ins].paidia[j]=value;
                break;
            } 
            else if(x<=nodes[nodes[nod_ins].paidia[j]].paidia[0]){
				temp=nodes[nod_ins].paidia[j];
                nodes[nod_ins].paidia[j]=value;   
                value=temp; 
            }    
        }//close for
        //as the node is leaf has key = -1
        for(j=0;j<=3;j++){
			nodes[count_nod].key[j]=-1;              
        }
        nodes[count_nod].paidia[0]=x;
        
        // initialize the properties of this new leaf    
        for(j=1;j<=4;j++){
			nodes[count_nod].paidia[j]=-1;              
        } 
        nodes[count_nod].children=0;
        nodes[count_nod].injured=0;
        nodes[count_nod].parent=nod_ins;
        nodes[count_nod].is_deleted=0; //update flag to NOT deleted
        
        //make the appropriate changes for finger search
        int l;
        for (l=0;l<nodes[nod_ins].children;l++){
			nodes[nodes[nod_ins].paidia[l]].level[1]=nodes[nod_ins].paidia[l+1];
            //  printf("%d :%d-->%d\n",l,nodes[nod_ins].paidia[l],nodes[nod_ins].paidia[l+1]);
        }
        for (l=1;l<nodes[nod_ins].children;l++){
			nodes[nodes[nod_ins].paidia[l]].level[0]=nodes[nod_ins].paidia[l-1];
            // printf("%d :%d<--%d\n",l,nodes[nod_ins].paidia[l-1],nodes[nod_ins].paidia[l]);
        }
        //  printf("meta t 2 for");
        nodes[nodes[nod_ins].paidia[0]].level[0]=left;
        
        //if children of the parent node is more than 4 then they should be split
        if(nodes[nod_ins].children>4){                           
			split(nod_ins);
        }   
        count_nod++;  
                                       
    }//close if for element not already at the tree
    else {
		// printf("\n Given element is already at the tree %d ",i);  
    }
}//close access

//implement split children function  
void split(int v){
	int value,j,c,k;
    int temp,grandpapa,found_v=0;
    int  papa=nodes[v].parent;

     if(nodes[v].riza==1){ //if you split the root    
		for(;(nodes[count_nod].is_deleted==0);count_nod++){//find index for new candidate root
        }
        nodes[v].parent=count_nod; 
        nodes[count_nod].is_deleted=0;  
        nodes[v].riza=0;
        nodes[count_nod].riza=1;//new root at the tree
        nodes[count_nod].children=1;
        papa=count_nod;
        count_nod++;
      }

      //if node (say v) to be split is not the root
      value=nodes[v].key[1];
      nodes[v].children=2;
      for(j=0;j<=3;j++){   
		if(nodes[papa].key[j]==-1){
			nodes[papa].key[j]=value;
            break;
        } 
        else if(nodes[v].key[j]<=nodes[papa].key[j]){
			temp=nodes[papa].key[j];
            nodes[papa].key[j]=value;   
            value=temp; 
        }    
      }
      
      //create v' node 
      for(;(nodes[count_nod].is_deleted==0);count_nod++){//find index for v' node
      }
      nodes[count_nod].level[1]=nodes[v].level[1];
      nodes[v].level[1]=count_nod;
      nodes[count_nod].level[0]=v;
      
      //update the uncles of current v node, to see v' node
      for(j=0;j<=4;j++){
		if (nodes[papa].paidia[j]==v){
			found_v=1; 
            break;                       
        }                                        
      }
      if(found_v){
		nodes[papa].paidia[j+1]=count_nod;    
      } 
      else{
		nodes[papa].paidia[0]=v;
        nodes[papa].paidia[1]=count_nod;
      }                      
      
      //node count_nod (v') will get the 2 children of v
      for(j=2;j<=3;j++){
		nodes[count_nod].key[j-2]=nodes[v].key[j];
      } 
      //the remaining keys=-1 of count_nod (v')
      for(c=2;c<=3;c++){
		nodes[count_nod].key[c]=-1;              
      }
      for(j=2;j<=4;j++){
		nodes[count_nod].paidia[j-2]=nodes[v].paidia[j];     
        nodes[nodes[count_nod].paidia[j-2]].parent=count_nod;   
      }
      
      //the remaining children=-1
      for(c=3;c<=4;c++){
		nodes[count_nod].paidia[c]=-1;               
      }
      //the remaining key =-1 of v
      for(j=1;j<=3;j++){
		nodes[v].key[j]=-1;             
      }       
      //the remaining children =-1 of v
      for(c=2;c<=4;c++){
         nodes[v].paidia[c]=-1;               
      }                                        
      
      nodes[papa].children++;
      nodes[count_nod].parent=papa;
      nodes[count_nod].children=3;
      nodes[count_nod].injured=0;//v' is not injured by construction
      nodes[count_nod].is_deleted=0;
      
      if (nodes[papa].children>4){//spliting children may propagate splits on higher level of the tree
		split(papa);
      }
      count_nod++;
        
   }//end of split

//implement delete node function   
void delete_nod(int x){
	int found=access(x,-2);//try to access element to be deleted
    int temp,j;
    int nod_delete,grandpapa;
    
    if (found>0){ //if element to be deleted is actually at the tree
 
		nod_delete=nodes[found].parent; 
        //changes for finger search on node to be deleted
        int left = nodes[nodes[nod_delete].paidia[0]].level[0];
        int right = nodes[nodes[nod_delete].paidia[nodes[nod_delete].children-1]].level[1];
        //shift keys of father of node to be deleted
        for(j=0;j<=1;j++){      
			if(x<=nodes[nod_delete].key[j]){
				nodes[nod_delete].key[j]=nodes[nod_delete].key[j+1];
				nodes[nod_delete].key[j+1]=nodes[nod_delete].key[j+2];  
            }    
        }
        
        //shift the positions of its children as well
        for(j=0;j<=2;j++){
			if(nodes[found].paidia[0]<=nodes[nodes[nod_delete].paidia[j]].paidia[0]){               
				nodes[nod_delete].paidia[j]=nodes[nod_delete].paidia[j+1];
                nodes[nod_delete].paidia[j+1]=nodes[nod_delete].paidia[j+2];                
            }                  
        }
        nodes[nod_delete].children--;
        //set deletion flag to true
        nodes[found].is_deleted=1;
        
        count_nod=found;
        grandpapa=nodes[nod_delete].parent;

		if(nodes[nod_delete].children<2){
			//find brother of node to be deleted (v) 
            for(j=0;j<=2;j++){
				if(x<=nodes[grandpapa].key[j]){                                         
					break;
                } 
            }
			j++;//find the left brother of node v
			//make updates for finger search
			int l;
			for (l=0;l<nodes[nod_delete].children;l++){
				nodes[nodes[nod_delete].paidia[l]].level[1]=nodes[nod_delete].paidia[l+1];
			}
			for (l=1;l<=nodes[nod_delete].children;l++){
				nodes[nodes[nod_delete].paidia[l]].level[0]=nodes[nod_delete].paidia[l-1];
			}
			nodes[nodes[nod_delete].paidia[0]].level[0]=left;
			nodes[nodes[nod_delete].paidia[nodes[nod_delete].children-1]].level[1]=right;
        
			//deleting a node may cause share or fuse to the father of the node..
			if( nodes[nodes[grandpapa].paidia[j]].children>2){//call share function
				share(x,nod_delete,nodes[grandpapa].paidia[j]);
			}
			else{//call fuse function                                            
				fuse(x,nod_delete,nodes[grandpapa].paidia[j]);
			}
		}
	}
    else{
          //printf("Element %d is not found at the tree",x);   
    }
}//end of delete

//implement share function to share children of x between v and u nodes   
 void  share(int x,int v,int u){
	int  grandpapa=nodes[u].parent,allagh_deikt;
    int temp=0,j;
    //count_nod=nodes[u].paidia[0];  
       
	for(j=0;j<=2;j++){//get key of parent v that points to node v
		if(x<=nodes[grandpapa].key[j]){
			temp=nodes[grandpapa].key[j];
            nodes[grandpapa].key[j]=nodes[u].key[0];
            break;
        }  
    }   
    //update keys of v
    allagh_deikt=j+1; 
    for(j=allagh_deikt;j<=2;j++){
		nodes[grandpapa].key[j]=nodes[grandpapa].key[j+1];
    }      
    
    //children of v's parent do not change during sharing
    nodes[v].key[0]=temp;
    nodes[v].paidia[1]=nodes[u].paidia[0]; //this child will go to v    
    nodes[nodes[v].paidia[1]].parent=v;
    nodes[v].children=2;
    
    //updating node v  
    for(j=1;j<=2;j++){
		nodes[v].key[j]=-1;
    }
    for(j=2;j<=3;j++){ 
		nodes[v].paidia[j]=-1;  
    } 
    
    //updating node u
    nodes[u].key[0]=nodes[u].key[1];     
    for(j=1;j<=2;j++){
		nodes[u].key[j]=-1;             
    }       
    for(j=0;j<=3;j++){
		nodes[u].paidia[j]=nodes[u].paidia[j+1];             
    }          
    nodes[u].children=2;
        
}//end of share

//implement fuse function to fuse nodes u and v
void  fuse(int x,int v,int u){
	int  grandpapa=nodes[u].parent,allagh_deikt;
    int temp,j;
    
    for(j=0;j<=2;j++){
		if(x<=nodes[grandpapa].key[j]){
			break;                            
        }                                          
    }
    //node v gets all information of node u
    nodes[v].key[0]=nodes[grandpapa].key[j];
    nodes[v].key[1]=nodes[u].key[0];
    nodes[v].paidia[1]=nodes[u].paidia[0];
    nodes[v].paidia[2]=nodes[u].paidia[1];
    nodes[nodes[v].paidia[1]].parent=v;
    nodes[nodes[v].paidia[2]].parent=v;
    nodes[v].children=3;
    allagh_deikt=j;
    
    //update keys of parent or u and v
    for(j=allagh_deikt;j<=2;j++){
		nodes[grandpapa].key[j]=nodes[grandpapa].key[j+1];           
    }      
    for(j=allagh_deikt+1;j<=3;j++){
        nodes[grandpapa].paidia[j]=nodes[grandpapa].paidia[j+1];
    } 
    
    //set up node u as deleted..
    count_nod=u;
    for(j=0;j<=3;j++){
		nodes[count_nod].key[j]=-1;              
    }
    for(j=1;j<=3;j++){
        nodes[count_nod].paidia[j]=-1;
    }       
    nodes[u].is_deleted=1;
    nodes[v].level[1]=nodes[u].level[1];
}//end of fuse                                  
