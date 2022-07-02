#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum { False, True}status;
typedef enum {Plus,Minus}sign;

typedef struct
{
	int *data;
	int len;
	sign s;
	
}Bigint; 

Bigint *new_int()
{
	Bigint *str =  malloc(sizeof(*str));
	return str;
}

void free_int(Bigint *num)
{
	free(num->data);
	free(num);
}

status smaller(Bigint *a ,Bigint *b)
{
	int i;
	if(a->len < b->len) 
	return True;
	if(b->len < a->len) 
	return False;
	for(i=0; i<a->len; i++)
	{
		if(a->data[i]<b->data[i]) 
		return True;
		else if(a->data[i]>b->data[i]) 
		return False;
	}
	return False;
}

void intialise(char *str,Bigint *x)
{

	int i;
	x->len=strlen(str);
	x->data=malloc ( (x->len) * sizeof(int));
	for(i=0; i<x->len; i++)
	{
	  x->data[i]=str[i]-'0';
	}
	if(str[0]=='-') 
	{
	  x->s=Minus;
	  x->data[0]=0;
    }
	else x->s=Plus;
	
}

int nonzero(Bigint *a)
{    
    int i;
	for(i=0; i<a->len; i++)
	{
		if(a->data[i]!=0) 
		return i;
	}
}

Bigint *add(Bigint *n1,Bigint *n2) 
{
    Bigint *result=new_int();
	result->len=(n1->len>=n2->len?n1->len:n2->len)+1;// terinary ex
	result->data = malloc(result->len * sizeof(int));// dynamic memory allocation for resultulting data 
	int i=n1->len-1,j=n2->len-1,k=result->len-1,carry=0;
	if(n1->s==n2->s) //sign 
	{
        while(k>=0)
        {
            int temp=carry;
            if(i>=0) temp+=n1->data[i];
            if(j>=0) temp+=n2->data[j];
   	        carry=temp/10;
   	        result->data[k]=temp%10;
   	        k--;
	        i--; 
	        j--;
        }
        result->s=n1->s;
	    return result;
	}

	else 
	{ 
	    Bigint *temp=new_int();
	    if(smaller(n1,n2))
        {
    	    temp=n1;
    	    n1=n2;
    	    n2=temp;
	    }
	    int i=n1->len-1,j=n2->len-1,k=result->len-1,carry=0;
	    result->s=n1->s;
	    while(k>=0)
	    {
		   int sub=0;
		    if(i>=0&&j>=0)
		    {
			    sub=n1->data[i]-n2->data[j]-carry;
			    if(sub<0)
			    {
				    sub+=10;
				    carry=1;
			    }
			    else carry=0;
		    }
		    else if(i>=0)
		    {
			    sub=n1->data[i]-carry;
			    if(sub<0)
			    {
				    sub+=10;
				    carry=1;
			    }
			    else carry=0;
		    }
		    result->data[k]=sub;
		    k--; i--; j--;
	    }
	    return result; 
    }
}

Bigint *subtract(Bigint *n1 , Bigint *n2)
{   
  
    Bigint *result=new_int();
    int flag=0;
	int small = smaller(n1,n2);
    result->len = (n1->len>=n2->len?n1->len:n2->len)+1;
    result->data = malloc(result->len * sizeof(int));  
	Bigint *temp =new_int();

	if(n1->s==Minus && n2->s==Plus) 
	    flag=1;
	else if(n1->s==Plus && n2->s==Minus)
	    flag=0;
	else if(n1->s==Plus && n2->s==Plus)
	{
		if(small)
		    flag=1;
		else
		    flag=0;
	}
	else
	{
        if(small)
		    flag=0;
		else
		    flag=1;
	}
    
	if(flag)
	    result->s=Minus;
	else
	    result->s=Plus;

    if(small)
    {
    	temp=n1;
    	n1=n2;
    	n2=temp;
	}
	
	int i=n1->len-1,j=n2->len-1, k=result->len-1,carry=0;
	
	if(n1->s==n2->s) 
	{
	    while(k>=0)
	    {   
            int sub=0;
		    if(i>=0&&j>=0)
		    {
			    sub=n1->data[i]-n2->data[j]-carry;
			    if(sub<0)
			    {
				    sub+=10;
				    carry=1;
			    }
			    else 
				    carry=0;
		    }
		    else if(i>=0)
		    {
			    sub=n1->data[i]-carry;
			    if(sub<0)
			    {
				    sub+=10;
				    carry=1;
			    }
			    else  
				    carry=0;
		    }
		    result->data[k]=sub;
		    k--; 
			i--;
			j--;
	    }
	    return result;
    }
    else 
    {
   	    while(k>=0)
        {
   	        int temp=carry;
   	        if(i>=0) temp+=n1->data[i];
   	        if(j>=0) temp+=n2->data[j];
   	        carry=temp/10;
   	        result->data[k]=temp%10;
   	        k--;
	        i--; 
	        j--;
        }
        return result;
    }
    
}


Bigint *multiply(Bigint *n1, Bigint *n2) 
{
    Bigint *result = new_int();
    result->len = n1->len + n2->len;
    result->data = malloc(result->len * sizeof(int));
	for(int i = 0; i <result->len; i++) 
	{ 
		result->data[i] = 0; 
	} 

    int i = n1->len - 1, j = n2->len - 1, k = result->len - 1;
    int carry = 0, temp, push_left = 0;
    while(i >= 0) 
	{
        k = result->len - 1 - push_left++;
        j = n2->len - 1;
        while(j >= 0 || carry > 0) 
		{
            if(j >= 0) 
			{
                temp = n1->data[i] * n2->data[j];
            } 
			else temp = 0;
            temp += carry;
            carry = temp / 10;
            result->data[k] += temp % 10;
            carry += result->data[k] / 10;
            result->data[k] = result->data[k] % 10;
            j--; k--;
        }
        i--;
    }
    if(n1->s^n2->s) result->s=Minus;
    else result->s=Plus;
    return result;
}

int main()
{ 
    Bigint *a =new_int();
    Bigint *b =new_int();
    char str1[500];
    char str2[500]; 
    scanf("%s",&str1);
    scanf("%s",&str2);
    intialise(str1,a);
    intialise(str2,b);

    Bigint *c = add(a,b);

    Bigint *d = subtract(a,b);
    Bigint *e = multiply(a,b);
    int i;

    printf("Addition of numbers is: ");
    if(c->s==Minus) printf("-");
    if(c->s==Plus) printf("+");
    for(i=nonzero(c); i< c->len; i++)
    {  
        printf("%d",c->data[i]);
    }
    printf("\n");

    printf("subtraction of number is: ");
    if(d->s==Minus) printf("-");
    if(d->s==Plus) printf("+");

    for(i=nonzero(d); i<d->len; i++)
    {
    	printf("%d",d->data[i]);
    }
    printf("\n");

    printf("multiplication is: ");
    if(e->s==Minus) printf("-");
    if(e->s==Plus) printf("+");
    for(i=nonzero(e); i<e->len; i++)
    {
    	printf("%d",e->data[i]);
    }
}